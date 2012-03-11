//============================================================================
// Name        : mm1.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in gtkmm
//============================================================================

#include <gtkmm.h>
#include <string>
#include "mmWindow.h"

#define MM_GCONF_BASE_PATH "/apps/aivan/mm1"

const Glib::ustring configBasePath(MM_GCONF_BASE_PATH);

int main(int argc, char *argv[]) {

	std::string configPath(configBasePath);
	configPath.append("/").append("MainWindow");

	Gtk::Main kit(argc, argv);

	mmWindow window;

	window.loadPosition(configPath);

	Gtk::Main::run(window);

	window.savePosition(configPath);

	return EXIT_SUCCESS;
}

