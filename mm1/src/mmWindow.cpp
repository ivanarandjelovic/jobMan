/*
 * mmWindow.cpp
 *
 *  Created on: Mar 11, 2012
 *      Author: aivan
 */

#include "mmWindow.h"

mmWindow::mmWindow() {
	isMaximized = false;

	positionValid = false;

	// We need this to precisely return window to it's previous possition
//	set_gravity(Gdk::GRAVITY_STATIC);
//
//	scrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
//	scrolledWindow.add(treeView);
//	treeModel = Gtk::ListStore::create(modelColumns);
//
//	treeView.set_model(treeModel);
//	//add(scrolledWindow);
//
//	m_VBox.pack_start(scrolledWindow);
//	m_VBox.pack_start(detailsLabel, Gtk::PACK_SHRINK);
//	add(m_VBox);

	add(paned);
	detailsLabel.set_text("<empty>");

	scrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	scrolledWindow.add(treeView);

	treeModel = Gtk::ListStore::create(modelColumns);

	treeView.set_model(treeModel);
	treeView.signal_selection_received().connect(sigc::mem_fun(*this, &mmWindow::on_job_selected));

	paned.pack1(scrolledWindow, true, true);
	paned.pack2(detailsLabel, true, true);

	show_all_children();

	// This seems to be not needed, I would really like to see some half-decent documentation
//	this->signal_window_state_event().connect(sigc::mem_fun(*this, &mmWindow::on_window_state_event));
}

void mmWindow::on_job_selected(const Gtk::SelectionData&, const unsigned int&) {
	g_message("aha");
}

/**
 * Save window position related details under provided path
 */
void mmWindow::savePosition(const Glib::ustring &windowConfPath) {
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

}

void mmWindow::loadPosition(const Glib::ustring &windowConfPath) {
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
}

int mmWindow::loadConfInt(Glib::RefPtr<Gnome::Conf::Client> &gConfClient, const Glib::ustring &windowConfPath,
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

bool mmWindow::loadConfBool(Glib::RefPtr<Gnome::Conf::Client> &gConfClient, const Glib::ustring &windowConfPath,
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

void mmWindow::setPosition() {
	if (positionValid) {
		resize(size_width, size_height);
		move(pos_x, pos_y);
		if (isMaximized) {
			maximize();
		} else {
			unmaximize();
		}
	}

	// Adjust panel and list:
//	treeView.set_size_request(size_width * 2 / 3, size_height);
//	scrolledWindow.set_size_request(size_width * 2 / 3, size_height);
	paned.set_position(size_width * 2 / 3);
}

bool mmWindow::on_configure_event(GdkEventConfigure* event) {
	if (!isMaximized) {
		size_width = event->width;
		size_height = event->height;
		pos_x = event->x;
		pos_y = event->y;
		positionValid = true;
	}
	return false;
}

bool mmWindow::on_window_state_event(GdkEventWindowState* event) {

	if (event->changed_mask & GDK_WINDOW_STATE_MAXIMIZED) {
		isMaximized = event->new_window_state & GDK_WINDOW_STATE_MAXIMIZED;
	}
	return false;
}

void mmWindow::loadServices(Services &services) {

	// Add data into the model
	for (std::list<Job>::iterator it = services.upstartJobs.begin(); it != services.upstartJobs.end(); it++) {
		Gtk::TreeModel::Row row = *(treeModel->append());
		row[modelColumns.jobName] = it->name;
		row[modelColumns.description] = it->description;
	}

	// Show columns in the vew
	treeView.append_column("Name", modelColumns.jobName);
	treeView.append_column("Description", modelColumns.description);
}
