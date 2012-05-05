//============================================================================
// Name        : main.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in gtkmm
//============================================================================

#include "main.h"

#include <gtkmm.h>
#include <string>
#include "jobManWindow.h"
#include "services.h"

#define JOBMAN_GCONF_BASE_PATH "/apps/aivan/jobMan"

const Glib::ustring configBasePath(JOBMAN_GCONF_BASE_PATH);

int main(int argc, char *argv[]) {

	// Start: Setting up memory profiling

	/* Set the GMemVTable to the default table. This needs to be called before
	* any other call to a GLib function. */
	//g_mem_set_vtable (glib_mem_profiler_table);

	/* Call g_mem_profile() when the application exits. */
	//g_atexit (g_mem_profile);

	// END: Setting up memory profiling


	std::string configPath(configBasePath);
	configPath.append("/").append("MainWindow");

	Gtk::Main kit(argc, argv);

	jobManWindow window;

	window.loadServices();

	window.loadPosition(configPath);

	Gtk::Main::run(window);

	window.savePosition(configPath);

	return EXIT_SUCCESS;
}

