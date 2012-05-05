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

#include "job.h"

using namespace Glib;
using namespace Gio;



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
	Glib::ustring formatBinom(std::vector<std::vector<Glib::ustring> > &vectorOfVectors);
};

#endif /* SERVICES_H_ */
