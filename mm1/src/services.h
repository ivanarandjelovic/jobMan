/*
 * services.h
 *
 *  Created on: Mar 12, 2012
 *      Author: aivan
 */

#ifndef SERVICES_H_
#define SERVICES_H_

#include <giomm.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <functional>

using namespace Glib;
using namespace Gio;

/**
 * Respresents one running instance of the upstart job
 */
class JobInstance {
public:
	Glib::ustring dbusObjectPath;
	Glib::ustring goal;
	Glib::ustring name;
	Glib::ustring state;
	JobInstance(Glib::ustring dbusObjectPath) {
		this->dbusObjectPath = dbusObjectPath;
	}
};

class Job {
public:
	Glib::ustring dbusObjectPath;
	Glib::ustring startOn;
	Glib::ustring stopOn;
	Glib::ustring emits;
	Glib::ustring author;
	Glib::ustring description;
	Glib::ustring version;
	Glib::ustring name;
	std::vector<JobInstance> instances;
	bool someInstanceRunning;

	Glib::ustring toString();
	Job() :
			someInstanceRunning(false) {
	}
};

class Services {

public:
	~Services();
	void loadUpstartJobs();
	void loadSysVJobs();

	std::vector<Job> upstartJobs;

private:

	Glib::ustring getStringProperty(RefPtr<DBus::Proxy> &jobProxy, const ustring &propertyName);
	Glib::ustring getStringArrayProperty(RefPtr<DBus::Proxy> &jobProxy, const ustring &propertyName);
	Glib::ustring getArrayOfStringArraysProperty(RefPtr<DBus::Proxy> &jobProxy, const ustring &propertyName);
	std::vector<JobInstance> readStructureWithArray(Glib::VariantContainerBase variantContainer);
	void loadInstance(RefPtr<DBus::Proxy> &jobProxy, JobInstance &jobInstance);
};

#endif /* SERVICES_H_ */
