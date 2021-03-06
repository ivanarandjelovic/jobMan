/*
 * jobManWindow.cpp
 *
 *  Created on: Mar 11, 2012
 *      Author: aivan
 */

#include "jobManWindow.h"

jobManWindow::jobManWindow() :
		refreshWorker(services) {
	isMaximized = false;
	positionValid = false;
	waitCursorCounter = 0;

	refreshWorker.sig_done.connect(sigc::mem_fun(*this, &jobManWindow::on_refresh_complete));
	startWorker.sig_done.connect(sigc::mem_fun(*this, &jobManWindow::on_start_complete));
	stopWorker.sig_done.connect(sigc::mem_fun(*this, &jobManWindow::on_stop_complete));

	// We need this to precisely return window to it's previous possition
	set_gravity(Gdk::GRAVITY_STATIC);

	// Prepare menu etc.
	//Create actions for menus and toolbars:
	refActionGroup = Gtk::ActionGroup::create();

	refActionGroup->add(Gtk::Action::create("FileMenu", "_File"));
	refActionGroup->add(Gtk::Action::create("FileExit", Gtk::Stock::QUIT, "E_xit", "Exit the application"),
			sigc::mem_fun(*this, &jobManWindow::on_menu_file_exit));

	refActionGroup->add(Gtk::Action::create("HelpMenu", "_Help"));
	refActionGroup->add(Gtk::Action::create("HelpAbout", Gtk::Stock::ABOUT, "_About", "About JobMan"),
			sigc::mem_fun(*this, &jobManWindow::on_menu_help_about));

	refUIManager = Gtk::UIManager::create();
	refUIManager->insert_action_group(refActionGroup);

	add_accel_group(refUIManager->get_accel_group());

	Glib::ustring ui_info = "<ui>"
			"  <menubar name='MenuBar'>"
			"    <menu action='FileMenu'>"
			"      <separator/>"
			"      <menuitem action='FileExit'/>"
			"    </menu>"
			"    <menu action='HelpMenu'>"
			"      <menuitem action='HelpAbout'/>"
			"    </menu>"
			"  </menubar>"
			"</ui>";

	try {
		refUIManager->add_ui_from_string(ui_info);
	} catch (const Glib::Error& ex) {
		std::cerr << "building menus failed: " << ex.what();
	}

	Gtk::Widget* pMenubar = refUIManager->get_widget("/MenuBar");
	if (pMenubar) {
		vMainBox.pack_start(*pMenubar, Gtk::PACK_SHRINK);
	}
	vMainBox.pack_start(paned, Gtk::PACK_EXPAND_WIDGET);

	add(vMainBox);

	paned.set_hexpand(true);
	paned.set_vexpand(true);

	scrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	scrolledWindow.add(treeView);

	treeModel = Gtk::ListStore::create(modelColumns);

	treeView.set_model(treeModel);
	//treeView.signal_selection_received().connect(sigc::mem_fun(*this, &mmWindow::on_job_selected));
	treeView.get_selection()->signal_changed().connect(sigc::mem_fun(*this, &jobManWindow::on_job_selected_handler));

	// Show columns in the vew
	treeView.append_column("Name", modelColumns.jobName);
	treeView.get_column(0)->set_expand(true);
	treeView.append_column("Running", modelColumns.someInstanceRunning);
	treeView.append_column("Manual\n(Disabled)", modelColumns.setToManual);
	treeView.append_column("Description", modelColumns.description);
	treeView.get_column(3)->set_expand(true);

	paned.pack1(scrolledWindow, Gtk::FILL);

	scrolledWindowLabel.add(detailsLabel);

	hBoxRightButtons.pack_start(buttonStart, true, true, 2);
	hBoxRightButtons.pack_start(buttonRestart, true, true, 2);
	hBoxRightButtons.pack_start(buttonStop, true, true, 2);

	hBoxRightLower.pack_start(buttonSetManual, true, true, 2);

	hBoxRightLower.pack_start(buttonReefresh, true, true, 2);

	vBoxRightOuter.pack_start(scrolledWindowLabel, true, true, 2); // add(detailsLabel);
	vBoxRightOuter.pack_start(hBoxRightButtons, false, true, 2);
	vBoxRightOuter.pack_start(hBoxRightLower, false, true, 2);

	buttonStart.set_label("Start");
	buttonRestart.set_label("Restart");
	buttonStop.set_label("Stop");
	buttonSetManual.set_label("Enable (remove manual)");
	buttonReefresh.set_label("Refresh list");

	// Initially buttons are disabled, and label is empty:
	initRightPanel();

	buttonReefresh.signal_clicked().connect(sigc::mem_fun(*this, &jobManWindow::on_refresh_clicked));
	buttonStart.signal_clicked().connect(sigc::mem_fun(*this, &jobManWindow::on_start_clicked));
	buttonRestart.signal_clicked().connect(sigc::mem_fun(*this, &jobManWindow::on_restart_clicked));
	buttonStop.signal_clicked().connect(sigc::mem_fun(*this, &jobManWindow::on_stop_clicked));
	buttonSetManual.signal_clicked().connect(sigc::mem_fun(*this, &jobManWindow::on_set_manual_clicked));

	//detailsLabel.set_markup("&lt;empty&gt;");
	detailsLabel.set_line_wrap(true);
	detailsLabel.set_use_markup(true);
	detailsLabel.set_alignment(Gtk::ALIGN_START, Gtk::ALIGN_START);

	paned.pack2(vBoxRightOuter, Gtk::FILL);

	services.loadUpstartJobs();

	show_all_children();

}

void jobManWindow::on_menu_file_exit() {
	hide();
}

void jobManWindow::on_menu_help_about() {
	Gtk::AboutDialog aboutDialog;
	std::vector<Glib::ustring> authors;
	authors.push_back("Ivan Arandjelovic <ivan.arandjelovic@gmail.com>");
	aboutDialog.set_authors(authors);
	aboutDialog.set_comments("Upstart Job Manager (wanabee)");
	aboutDialog.set_version(JOBMAN_VERSION);
	aboutDialog.run();
	aboutDialog.hide();
}

void jobManWindow::initRightPanel() {
	// Initially buttons are disabled, and label is empty:
	detailsLabel.set_markup("");
	buttonStart.set_sensitive(false);
	buttonRestart.set_sensitive(false);
	buttonStop.set_sensitive(false);
	buttonSetManual.set_sensitive(false);
}

void jobManWindow::on_job_selected_handler() {
	//g_message("aha");

	Gtk::TreeIter iter = treeView.get_selection()->get_selected();
	if (iter) {
		//std::cout << iter->get_value(modelColumns.jobName) << std::endl;
		//std::cout << iter->get_value(modelColumns.completeDescription) << std::endl;
		detailsLabel.set_markup(iter->get_value(modelColumns.completeDescription));
		selectedJob = iter->get_value(modelColumns.job);
		if (selectedJob.someInstanceRunning) {
			buttonStop.set_sensitive(true);
			buttonRestart.set_sensitive(true);
			buttonStart.set_sensitive(false);
		} else {
			buttonStop.set_sensitive(false);
			buttonRestart.set_sensitive(false);
			buttonStart.set_sensitive(true);
		}
		if (selectedJob.canBeSetToManual) {
			if (selectedJob.manual) {
				buttonSetManual.set_label("Enable (remove manual)");
			} else {
				buttonSetManual.set_label("Disable (set manual)");
			}
			buttonSetManual.set_sensitive(true);
		} else {
			//buttonSetManual.set_label("")
			buttonSetManual.set_sensitive(false);
		}
	}
}

/**
 * Save window position related details under provided path
 */
void jobManWindow::savePosition(const Glib::ustring &windowConfPath) {
	Gnome::Conf::init();

	Glib::RefPtr<Gnome::Conf::Client> gConfClient(Gnome::Conf::Client::get_default_client());

	gConfClient->add_dir(windowConfPath, Gnome::Conf::CLIENT_PRELOAD_ONELEVEL);

	Glib::ustring key;

	key.erase().append(windowConfPath).append("/size_height");
	gConfClient->set(key, size_height);

	key.erase().append(windowConfPath).append("/size_width");
	gConfClient->set(key, size_width);

	key.erase().append(windowConfPath).append("/pos_x");
	gConfClient->set(key, pos_x);

	key.erase().append(windowConfPath).append("/pos_y");
	gConfClient->set(key, pos_y);

	key.erase().append(windowConfPath).append("/isMaximized");
	gConfClient->set(key, isMaximized);

	key.erase().append(windowConfPath).append("/panelSeparator");
	gConfClient->set(key, paned.get_position());

}

void jobManWindow::loadPosition(const Glib::ustring &windowConfPath) {
	Gnome::Conf::init();

	Glib::RefPtr<Gnome::Conf::Client> gConfClient(Gnome::Conf::Client::get_default_client());

	gConfClient->add_dir(windowConfPath, Gnome::Conf::CLIENT_PRELOAD_ONELEVEL);

	Glib::ustring key;
	Gnome::Conf::Value value;

// It seems rather wrong to assume operation was OK, and then to mark it as failed
// later if we encounter some problems, but for now I do it this way (need more C++ practice :) )
	positionValid = true;
	size_height = loadConfInt(gConfClient, windowConfPath, "/size_height");
	size_width = loadConfInt(gConfClient, windowConfPath, "/size_width");
	pos_x = loadConfInt(gConfClient, windowConfPath, "/pos_x");
	pos_y = loadConfInt(gConfClient, windowConfPath, "/pos_y");
	isMaximized = loadConfBool(gConfClient, windowConfPath, "/isMaximized");

	setPosition();

	int panelSeparator = loadConfInt(gConfClient, windowConfPath, "/panelSeparator");
	if (positionValid) {
		paned.set_position(panelSeparator);
	} else {
		paned.set_position(size_width * 2 / 3);
	}
}

int jobManWindow::loadConfInt(Glib::RefPtr<Gnome::Conf::Client> &gConfClient, const Glib::ustring &windowConfPath,
		const Glib::ustring &keyName) {
	Glib::ustring key(windowConfPath);
	Gnome::Conf::Value value;
	int returnValue = 0;

	key.append(keyName);
	value = gConfClient->get(key);
	if (value.get_type() != Gnome::Conf::VALUE_INVALID) {
		returnValue = value.get_int();
	} else {
		positionValid = false;
	}
	return returnValue;
}

bool jobManWindow::loadConfBool(Glib::RefPtr<Gnome::Conf::Client> &gConfClient, const Glib::ustring &windowConfPath,
		const Glib::ustring &keyName) {
	Glib::ustring key(windowConfPath);
	Gnome::Conf::Value value;
	bool returnValue = 0;

	key.append(keyName);
	value = gConfClient->get(key);
	if (value.get_type() != Gnome::Conf::VALUE_INVALID) {
		returnValue = value.get_bool();
	} else {
		positionValid = false;
	}
	return returnValue;
}

/**
 *  Move and resize the window to saved values, not checking if it is offscren, wm seems
 *  to be taking care of this
 */

void jobManWindow::setPosition() {
	if (positionValid) {
		resize(size_width, size_height);
		move(pos_x, pos_y);
		if (isMaximized) {
			maximize();
		} else {
			unmaximize();
		}
	} else {
		resize(640 * 1.5, 400 * 1.5);
		set_position(Gtk::WIN_POS_CENTER);
	}
}

bool jobManWindow::on_configure_event(GdkEventConfigure* event) {
	if (!isMaximized) {
		size_width = event->width;
		size_height = event->height;
		pos_x = event->x;
		pos_y = event->y;
		positionValid = true;
	}
	// Never forget to call parent method
	return Gtk::Window::on_configure_event(event);
}

bool jobManWindow::on_window_state_event(GdkEventWindowState* event) {

	if (event->changed_mask & GDK_WINDOW_STATE_MAXIMIZED) {
		isMaximized = event->new_window_state & GDK_WINDOW_STATE_MAXIMIZED;
	}
	// Never forget to call parent method
	return Gtk::Window::on_window_state_event(event);
}

void jobManWindow::loadServices() {

	treeModel->clear();

// Add data into the model
	for (std::vector<Job>::iterator it = services.upstartJobs.begin(); it != services.upstartJobs.end(); it++) {
		Gtk::TreeModel::Row row = *(treeModel->append());
		row[modelColumns.job] = *it;
		row[modelColumns.jobName] = it->name;
		row[modelColumns.description] = it->description;
		row[modelColumns.someInstanceRunning] = it->someInstanceRunning;
		row[modelColumns.setToManual] = it->manual;
		row[modelColumns.completeDescription] = it->toString();
	}

}

void jobManWindow::setWaitCursor() {
	if (waitCursorCounter == 0) {
		g_debug("Setting wait cursor...");
		Glib::RefPtr<Gdk::Cursor> oldCursor = get_window()->get_cursor();
		get_window()->set_cursor(Gdk::Cursor::create(Gdk::WATCH));
	}
	waitCursorCounter++;
}

void jobManWindow::unsetWaitCursor() {
	waitCursorCounter--;
	if (waitCursorCounter <= 0) {
		g_debug("Unsetting wait cursor...");
		get_window()->set_cursor();
	}
}

void jobManWindow::on_refresh_clicked() {
	this->set_sensitive(false);
	refreshMutex.lock();
	setWaitCursor();
	refreshWorker.start();
}

void jobManWindow::on_refresh_complete() {
	loadServices();
	initRightPanel();
	unsetWaitCursor();
	refreshMutex.unlock();
	this->set_sensitive(true);
}

void jobManWindow::on_start_clicked() {
	this->set_sensitive(false);
	setWaitCursor();
	startWorker._selectedJob = &selectedJob;
	startWorker.start();
}

void  jobManWindow::on_start_complete() {
	unsetWaitCursor();
	this->set_sensitive(true);
	on_refresh_clicked();
}

void jobManWindow::on_restart_clicked() {
	setWaitCursor();
	selectedJob.restart();
	on_refresh_clicked();
	unsetWaitCursor();
}
void jobManWindow::on_stop_clicked() {
	this->set_sensitive(false);
	setWaitCursor();
	stopWorker._selectedJob = &selectedJob;
	stopWorker.start();
}
void jobManWindow::on_stop_complete() {
	unsetWaitCursor();
	this->set_sensitive(true);
	on_refresh_clicked();
}

void jobManWindow::on_set_manual_clicked() {
	setWaitCursor();
	selectedJob.setManual();
	on_refresh_clicked();
	unsetWaitCursor();
}
