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
	Gtk::TreeModelColumn<Glib::ustring> jobName;
	Gtk::TreeModelColumn<Glib::ustring> description;
	Gtk::TreeModelColumn<Glib::ustring> completeDescription;
	//Gtk::TreeModelColumn< Glib::RefPtr<Gdk::Pixbuf> > thumbnail;

	MyModelColumns() {
		add(jobName);
		add(description);
		add(completeDescription);
		//add (thumbnail);
	}
};

class mmWindow: public Gtk::Window {

public:
	mmWindow();
	bool on_configure_event(GdkEventConfigure* event);
	bool on_window_state_event(GdkEventWindowState* event);

	void savePosition(const Glib::ustring &windowConfPath);
	void loadPosition(const Glib::ustring &windowConfPath);

	void loadServices(Services &services);

private:
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
	Gtk::TreeView treeView;
	MyModelColumns modelColumns;
	Glib::RefPtr<Gtk::ListStore> treeModel;

	Gtk::Label detailsLabel;
	//Gtk::Box m_VBox;

protected:
	// events:
	void on_job_selected();
};

#endif /* MMWINDOW_H_ */
