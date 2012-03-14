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

	void toOutput();
};

class Services {

public:
	~Services();
	void loadUpstartJobs();
	void loadSysVJobs();
private:
	std::list<Job> upstartJobs;

	Glib::ustring getStringProperty(RefPtr<DBus::Proxy> &jobProxy, const ustring &propertyName);
	Glib::ustring getStringArrayProperty(RefPtr<DBus::Proxy> &jobProxy, const ustring &propertyName);

};

#endif /* SERVICES_H_ */
