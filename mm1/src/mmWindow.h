/*
 * mmWindow.h
 *
 *  Created on: Mar 11, 2012
 *      Author: aivan
 */

#ifndef MMWINDOW_H_
#define MMWINDOW_H_

#include <gtkmm.h>
#include <gconfmm.h>
#include <memory>

#include "services.h"

class MyModelColumns: public Gtk::TreeModel::ColumnRecord {
public:
	Gtk::TreeModelColumn<Job> job;
	Gtk::TreeModelColumn<Glib::ustring> jobName;
	Gtk::TreeModelColumn<Glib::ustring> description;
	Gtk::TreeModelColumn<bool> someInstanceRunning;
	Gtk::TreeModelColumn<Glib::ustring> completeDescription;
	//Gtk::TreeModelColumn< Glib::RefPtr<Gdk::Pixbuf> > thumbnail;

	MyModelColumns() {
		add(job);
		add(jobName);
		add(description);
		add(someInstanceRunning);
		add(completeDescription);
		//add (thumbnail);
	}
};

class mmWindow: public Gtk::Window {

public:
	mmWindow();

	void savePosition(const Glib::ustring &windowConfPath);
	void loadPosition(const Glib::ustring &windowConfPath);

	void loadServices();

private:
	Services services;
	Job selectedJob;
	bool isMaximized;
	int size_width, size_height, pos_x, pos_y;
	bool positionValid;

	void setPosition();
	int loadConfInt(Glib::RefPtr<Gnome::Conf::Client> &gConfClient, const Glib::ustring &windowConfPath,
			const Glib::ustring &keyName);
	bool loadConfBool(Glib::RefPtr<Gnome::Conf::Client> &gConfClient, const Glib::ustring &windowConfPath,
			const Glib::ustring &keyName);

	// Widgets
	Gtk::Paned paned;

	Gtk::ScrolledWindow scrolledWindow;
	Gtk::ScrolledWindow scrolledWindowLabel;

	MyModelColumns modelColumns;

	Gtk::TreeView treeView;
	Glib::RefPtr<Gtk::ListStore> treeModel;

	Gtk::VBox vBoxRightOuter;
	Gtk::HBox hBoxRightButtons;
	Gtk::HBox hBoxRightLower;
	Gtk::Button buttonStart;
	Gtk::Button buttonRestart;
	Gtk::Button buttonStop;
	Gtk::Button buttonReefresh;
	Gtk::Label detailsLabel;
	//Gtk::Box m_VBox;

protected:
	// events:
	void on_job_selected_handler();
	void on_refresh_clicked();

	// Overriden
	bool on_configure_event(GdkEventConfigure* event);
	bool on_window_state_event(GdkEventWindowState* event);

};

#endif /* MMWINDOW_H_ */
