/*
 * job.h
 *
 *  Created on: Mar 19, 2012
 *      Author: aivan
 */

#ifndef JOB_H_
#define JOB_H_

#include <giomm.h>
//#include <iostream>
//#include <sstream>
//#include <algorithm>
//#include <functional>

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

	bool start();
	bool restart();
	bool stop();
};

#endif /* JOB_H_ */
