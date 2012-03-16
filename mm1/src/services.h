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

using namespace Glib;
using namespace Gio;

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
	std::vector<Glib::ustring> instances;

	Glib::ustring toString();
};

class Services {

public:
	~Services();
	void loadUpstartJobs();
	void loadSysVJobs();

	std::list<Job> upstartJobs;
private:


	Glib::ustring getStringProperty(RefPtr<DBus::Proxy> &jobProxy, const ustring &propertyName);
	Glib::ustring getStringArrayProperty(RefPtr<DBus::Proxy> &jobProxy, const ustring &propertyName);
	Glib::ustring getArrayOfStringArraysProperty(RefPtr<DBus::Proxy> &jobProxy, const ustring &propertyName);
	std::vector<Glib::ustring> readStringContainer(Glib::VariantContainerBase variantContainer);
};

#endif /* SERVICES_H_ */