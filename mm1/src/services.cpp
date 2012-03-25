/*
 * services.cpp
 *
 *  Created on: Mar 12, 2012
 *      Author: aivan
 */

#include "services.h"

using namespace std;





Services::~Services() {
	//g_message("at end, upstart jobs listed START:");
	//for (std::vector<Job>::iterator it = upstartJobs.begin(); it != upstartJobs.end(); it++) {
	//	cout << it->toString() << endl;
	//}
	//g_message("at end, upstart jobs listed END.");
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

	VariantContainerBase variantContainer;

	jobProxy->get_cached_property(variantContainer, propertyName);

	VariantIter iterator(variantContainer);

	Variant<std::vector<Glib::ustring> > innerVectorContainer;
	int outerLoop = 0;
	while (iterator.next_value(innerVectorContainer)) {
		if (outerLoop > 0) {
			propertyValue.append("\n");
		}
		std::vector<Glib::ustring> innerVector = innerVectorContainer.get();
		for (uint j = 0; j < innerVector.size(); j++) {
			if (j > 0) {
				propertyValue.append(", ");
			}
			propertyValue.append(innerVector.at(j));
		}
		outerLoop++;
	}

	return propertyValue;
}

std::vector<JobInstance> Services::readStructureWithArray(Glib::VariantContainerBase variantContainer) {
	std::vector<JobInstance> result;
	//cout << variantContainer.get_type_string() << endl;
	VariantIter iterator(variantContainer);

	VariantBase instance;
	while (iterator.next_value(instance)) {
		VariantIter iterator2(instance);
		Variant<Glib::ustring> instance2;
		while (iterator2.next_value(instance2)) {
			//cout << instance2.get_type_string() << endl;
			//cout << instance2.get() << endl;
			JobInstance jobInstace(instance2.get());
			result.push_back(jobInstace);
		}

	}

	return result;
}

class CompareJobs {
public:
	bool operator()(const Job &j1, const Job &j2) const {
		return (j1.name) < (j2.name);
	}
};

void Services::loadInstance(RefPtr<DBus::Proxy> &jobProxy, JobInstance &jobInstance) {
	jobInstance.goal = getStringProperty(jobProxy, "goal");
	jobInstance.name = getStringProperty(jobProxy, "name");
	jobInstance.state = getStringProperty(jobProxy, "state");
}

void Services::loadUpstartJobs() {
	// Clear previously loaded jobs
	upstartJobs.clear();

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

		//g_message("job: '%s", jobObjectPath.c_str());

		Job job;
		job.dbusObjectPath = jobObjectPath;

		// Load properties of this job:
		RefPtr<DBus::Proxy> jobProxy = DBus::Proxy::create_sync(busConnection, "com.ubuntu.Upstart", jobObjectPath,
				"com.ubuntu.Upstart0_6.Job");

		job.startOn = getArrayOfStringArraysProperty(jobProxy, ustring("start_on"));
		job.stopOn = getArrayOfStringArraysProperty(jobProxy, ustring("stop_on"));
		job.emits = getStringArrayProperty(jobProxy, ustring("emits"));
		job.author = getStringProperty(jobProxy, ustring("author"));
		job.description = getStringProperty(jobProxy, ustring("description"));
		job.version = getStringProperty(jobProxy, ustring("version"));
		job.name = getStringProperty(jobProxy, ustring("name"));

		job.loadManualOverrideSettings();

		VariantContainerBase variantContainerInstances;
		Glib::VariantContainerBase parameteres;
		variantContainerInstances = jobProxy->call_sync("GetAllInstances", parameteres);
		job.instances = readStructureWithArray(variantContainerInstances);

		for (std::vector<JobInstance>::iterator instanceIterator = job.instances.begin();
				instanceIterator != job.instances.end(); instanceIterator++) {
			RefPtr<DBus::Proxy> jobProxy = DBus::Proxy::create_sync(busConnection, "com.ubuntu.Upstart",
					instanceIterator->dbusObjectPath, "com.ubuntu.Upstart0_6.Instance");

			loadInstance(jobProxy, *instanceIterator);
			if (instanceIterator->state == "running") {
				job.someInstanceRunning = true;
			}
		}

//		cout << "whatIsThis : " << whatIsThis << endl;

		upstartJobs.push_back(job);
	}

	busConnection->close_sync();

	// sort services:
	sort(upstartJobs.begin(), upstartJobs.end(), CompareJobs());
}

void Services::loadSysVJobs() {

}

