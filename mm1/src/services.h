/*
 * services.h
 *
 *  Created on: Mar 12, 2012
 *      Author: aivan
 */

#ifndef SERVICES_H_
#define SERVICES_H_

#include <giomm.h>

class Services {

public:
	~Services();
	void loadUpstartJobs();
	void loadSysVJobs();
private:
	std::list<Glib::ustring> upstartJobNames;
	std::list<Glib::ustring> sysVJobNames;
};

#endif /* SERVICES_H_ */
