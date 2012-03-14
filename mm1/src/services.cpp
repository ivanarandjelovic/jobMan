/*
 * services.cpp
 *
 *  Created on: Mar 12, 2012
 *      Author: aivan
 */

#include "services.h"

using namespace std;

void Job::toOutput() {
	cout << "Job:" << endl;
	cout << "dbusObjectPath = " << dbusObjectPath << endl;
	cout << "startOn = " << startOn << endl;
	cout << "stopOn = " << stopOn << endl;
	cout << "emits = " << emits << endl;
	cout << "author = " << author << endl;
	cout << "description = " << description << endl;
	cout << "version = " << version << endl;
	cout << "name = " << name << endl;
}

Services::~Services() {
	g_message("at end, upstart jobs listed START:");
	for (std::list<Job>::iterator it = upstartJobs.begin(); it != upstartJobs.end(); it++) {
		it->toOutput();
	}
	g_message("at end, upstart jobs listed END.");
}

Glib::ustring Services::getStringProperty(RefPtr<DBus::Proxy> &jobProxy, const ustring &propertyName) {

	Variant<Glib::ustring> variantStringBase;
	jobProxy->get_cached_property(variantStringBase, propertyName);
	Glib::ustring propertyValue = variantStringBase.get();
	return propertyValue;
}

Glib::ustring Services::getStringArrayProperty(RefPtr<DBus::Proxy> &jobProxy, const ustring &propertyName) {

	Variant<std::vector<Glib::ustring> > variantStringVectorBase;
	jobProxy->get_cached_property(variantStringVectorBase, propertyName);
	Glib::ustring propertyValue;
	std::vector<Glib::ustring> vector = variantStringVectorBase.get();
	for (uint i = 0; i < vector.size(); i++) {
		if (i > 0) {
			propertyValue.append(", ");
		}
		propertyValue.append(vector.at(i));
	}
	return propertyValue;
}

Glib::ustring Services::getArrayOfStringArraysProperty(RefPtr<DBus::Proxy> &jobProxy, const ustring &propertyName) {
	Glib::ustring propertyValue;

	VariantContainerBase varianContainer;
	jobProxy->get_cached_property(varianContainer, propertyName);

	if (varianContainer.get_size() > 0) {

		VariantIter iterator(varianContainer.get_child(0));

		Variant<Glib::ustring> varArray;
		while (iterator.next_value(varArray)) {
			cout << varArray.get_type_string() << endl;
			propertyValue.append(", ").append(varArray.get());
//		std::vector<Glib::ustring> strVector = varArray.get();
//		for (uint j = 0; j < strVector.size(); j++) {
//			if (j > 0) {
//				propertyValue.append(", ");
//			}
//			propertyValue.append(strVector.at(j));
//		}
		}
	}
	return propertyValue;
}

void Services::loadUpstartJobs() {
	Glib::RefPtr<Gio::DBus::Connection> busConnection;

	Glib::init();
	Gio::init();

	busConnection = DBus::Connection::get_sync(Gio::DBus::BUS_TYPE_SYSTEM);

	// Dont kill our process when we close this connection
	busConnection->set_exit_on_close(false);

	RefPtr<DBus::Proxy> upstartProxy = DBus::Proxy::create_sync(busConnection, "com.ubuntu.Upstart",
			"/com/ubuntu/Upstart", "com.ubuntu.Upstart0_6");

	VariantContainerBase jobsVariant = upstartProxy->call_sync("GetAllJobs");
	VariantIter iterator(jobsVariant.get_child(0));

	Variant<ustring> var;
	while (iterator.next_value(var)) {
		ustring jobObjectPath = var.get();

		g_message("job: '%s", jobObjectPath.c_str());

		Job service;
		service.dbusObjectPath = jobObjectPath;

		// Load properties of this job:
		RefPtr<DBus::Proxy> jobProxy = DBus::Proxy::create_sync(busConnection, "com.ubuntu.Upstart", jobObjectPath,
				"com.ubuntu.Upstart0_6.Job");

		service.startOn = getArrayOfStringArraysProperty(jobProxy, ustring("start_on"));
		service.stopOn = getArrayOfStringArraysProperty(jobProxy, ustring("stop_on"));
		service.emits = getStringArrayProperty(jobProxy, ustring("emits"));
		service.author = getStringProperty(jobProxy, ustring("author"));
		service.description = getStringProperty(jobProxy, ustring("description"));
		service.version = getStringProperty(jobProxy, ustring("version"));
		service.name = getStringProperty(jobProxy, ustring("name"));

//		cout << "whatIsThis : " << whatIsThis << endl;

		upstartJobs.push_back(service);
	}

	busConnection->close_sync();
}

void Services::loadSysVJobs() {

}

