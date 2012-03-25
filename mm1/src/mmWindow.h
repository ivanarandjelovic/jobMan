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

#define JOBMAN_VERSION "0.1"

class MyModelColumns: public Gtk::TreeModel::ColumnRecord {
public:
	Gtk::TreeModelColumn<Job> job;
	Gtk::TreeModelColumn<Glib::ustring> jobName;
	Gtk::TreeModelColumn<Glib::ustring> description;
	Gtk::TreeModelColumn<bool> someInstanceRunning;
	Gtk::TreeModelColumn<bool> setToManual;
	Gtk::TreeModelColumn<Glib::ustring> completeDescription;
	//Gtk::TreeModelColumn< Glib::RefPtr<Gdk::Pixbuf> > thumbnail;

	MyModelColumns() {
		add(job);
		add(jobName);
		add(description);
		add(someInstanceRunning);
		add(setToManual);
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
	void initRightPanel();

	// Widgets
	Gtk::Paned paned;

	Gtk::ScrolledWindow scrolledWindow;
	Gtk::ScrolledWindow scrolledWindowLabel;

	MyModelColumns modelColumns;

	Gtk::TreeView treeView;
	Glib::RefPtr<Gtk::ListStore> treeModel;

	Gtk::VBox vMainBox;
	Gtk::VBox vBoxRightOuter;
	Gtk::HBox hBoxRightButtons;
	Gtk::HBox hBoxRightLower;
	Gtk::Button buttonStart;
	Gtk::Button buttonRestart;
	Gtk::Button buttonStop;
	Gtk::Button buttonSetManual;
	Gtk::Button buttonReefresh;
	Gtk::Label detailsLabel;
	//Gtk::Box m_VBox;

	Glib::RefPtr<Gtk::UIManager> refUIManager;
	Glib::RefPtr<Gtk::ActionGroup> refActionGroup;

protected:
	// events:
	void on_job_selected_handler();
	void on_refresh_clicked();
	void on_start_clicked();
	void on_restart_clicked();
	void on_stop_clicked();
	void on_menu_file_exit();
	void on_menu_help_about();
	void on_set_manual_clicked();

	// Overriden
	bool on_configure_event(GdkEventConfigure* event);
	bool on_window_state_event(GdkEventWindowState* event);

};

#endif /* MMWINDOW_H_ */
