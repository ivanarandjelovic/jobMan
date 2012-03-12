/*
 * services.cpp
 *
 *  Created on: Mar 12, 2012
 *      Author: aivan
 */

#include "services.h"

Services::~Services() {
	for (std::list<Glib::ustring>::iterator it = upstartJobNames.begin(); it != upstartJobNames.end(); it++) {
		g_message("at end, upstart job: %s",it->c_str());
	}
	upstartJobNames.clear();
	sysVJobNames.clear();
}

void Services::loadUpstartJobs() {
	Glib::RefPtr<Gio::DBus::Connection> busConnection;

	using namespace Glib;
	using namespace Gio;

	Glib::init();
	Gio::init();

	busConnection = DBus::Connection::get_sync(Gio::DBus::BUS_TYPE_SYSTEM);

	// Dont kill our process when we close this connection
	busConnection->set_exit_on_close(false);

	RefPtr<DBus::Proxy> udisks_proxy = DBus::Proxy::create_sync(busConnection, "com.ubuntu.Upstart",
			"/com/ubuntu/Upstart", "com.ubuntu.Upstart0_6");

	VariantContainerBase devices_variant = udisks_proxy->call_sync("GetAllJobs");
	VariantIter iterator(devices_variant.get_child(0));

	Variant<ustring> var;
	while (iterator.next_value(var)) {
		ustring name = var.get();

		g_message("job: '%s", name.c_str());
		upstartJobNames.push_back(name);
	}
	busConnection->close_sync();
}

void Services::loadSysVJobs() {

}

