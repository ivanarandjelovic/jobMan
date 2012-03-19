/*
 * job.cpp
 *
 *  Created on: Mar 19, 2012
 *      Author: aivan
 */

#include "job.h"

#define START_BOLD "<span weight=\"bold\">"
#define END_BOLD "</span>"

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

bool Job::start() {
	bool result = false;

	return result;
}

bool Job::restart() {
	if (stop()) {
		return start();
	} else {
		return false;
	}
}

bool Job::stop() {
	bool result = false;

	return result;

}

