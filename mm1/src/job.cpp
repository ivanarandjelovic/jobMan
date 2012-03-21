/*
 * job.cpp
 *
 *  Created on: Mar 19, 2012
 *      Author: aivan
 */

#include "job.h"

#define START_BOLD "<span weight=\"bold\">"
#define END_BOLD "</span>"

// timeout in ms for calling DBUS methods
#define DBUS_METHOD_TIMEOUT 60000

using namespace std;

Glib::ustring Job::toString() {
	Glib::ustring result;
	result.append(START_BOLD"Job:"END_BOLD"\n");
	result.append(START_BOLD"name"END_BOLD" = ").append(name).append("\n");
	result.append(START_BOLD"description"END_BOLD" = ").append(description).append("\n");
	result.append(START_BOLD"author"END_BOLD" = ").append(Glib::Markup::escape_text(author)).append("\n");
	result.append(START_BOLD"version"END_BOLD" = ").append(version).append("\n");
	result.append(START_BOLD"startOn"END_BOLD" = ").append(startOn).append("\n");
	result.append(START_BOLD"stopOn"END_BOLD" = ").append(stopOn).append("\n");
	result.append(START_BOLD"emits"END_BOLD" = ").append(emits).append("\n");
	result.append(START_BOLD"dbusObjectPath"END_BOLD" = ").append(dbusObjectPath).append("\n");
	result.append(START_BOLD"\ninstances:"END_BOLD"\n\n");
	for (std::vector<JobInstance>::iterator it = instances.begin(); it != instances.end(); it++) {
		bool useGreen = false;
		if (it->state == "running") {
			useGreen = true;
		}
		result.append(START_BOLD"  name"END_BOLD" =").append(it->name).append(",").append(
				(useGreen ? "<span color=\"green\">" : "")).append(it->state).append((useGreen ? "</span>" : "")).append(
				"\n");
	}
	result.append("\n");

	return result;
}

bool Job::startStop(bool start) {

	Glib::RefPtr<Gio::DBus::Connection> busConnection = Gio::DBus::Connection::get_sync(Gio::DBus::BUS_TYPE_SYSTEM);

	// Dont kill our process when we close this connection
	busConnection->set_exit_on_close(false);

	// Load properties of this job:
	Glib::RefPtr<Gio::DBus::Proxy> jobProxy = Gio::DBus::Proxy::create_sync(busConnection, "com.ubuntu.Upstart",
			dbusObjectPath, "com.ubuntu.Upstart0_6.Job");

	std::vector<Glib::ustring> emptyVector;
	Glib::Variant<std::vector<Glib::ustring> > arrayOfStrings = Glib::Variant<std::vector<Glib::ustring> >::create(
			emptyVector);
	Glib::Variant<bool> boolWait = Glib::Variant<bool>::create(true);

	std::vector<Glib::VariantBase> paramVector;
	paramVector.push_back(arrayOfStrings);
	paramVector.push_back(boolWait);

	Glib::VariantContainerBase parameters = Glib::VariantContainerBase::create_tuple(paramVector);

	if (start) {
		Glib::VariantContainerBase objectPathVariant = jobProxy->call_sync("Start", parameters, DBUS_METHOD_TIMEOUT,
				Gio::DBus::CALL_FLAGS_NONE);
	} else {
		jobProxy->call_sync("Stop", parameters, DBUS_METHOD_TIMEOUT, Gio::DBus::CALL_FLAGS_NONE);

	}
	// Seems that there is no need for this
	//busConnection->close_sync();

	return true;
}

bool Job::start() {
	return startStop(true);
}

bool Job::stop() {
	return startStop(false);
}

bool Job::restart() {
	if (stop()) {
		return start();
	} else {
		return false;
	}
}

