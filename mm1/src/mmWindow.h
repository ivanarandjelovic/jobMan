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
#include <glibmm.h>
#include <memory>

#include "services.h"

#define JOBMAN_VERSION "0.2"

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

class Worker {
public:
	bool active;

	Worker() :
			active(false) {
	}

	// Called to start the processing on the thread
	virtual void start() {
		active = true;
		Glib::Thread::create(sigc::mem_fun(*this, &Worker::internalStart), false);
	}

	// When shutting down, we need to stop the thread
	virtual ~Worker() {

	}

	virtual void run() = 0;

	Glib::Dispatcher sig_done;

protected:
	// This is where the real work happens
	void internalStart() {
		run();
		active = false;
		sig_done();
	}

};

class RefreshWorker: public Worker {
public:
	RefreshWorker(Services &services) :
			Worker(), _services(services) {
	}
	void run() {
		_services.loadUpstartJobs();
	}
protected:
	Services &_services;
};

class StartWorker: public Worker {
public:
	StartWorker() :
			Worker() {
	}
	void run() {
		_selectedJob->start();
	}
	Job *_selectedJob;
};

class StopWorker: public Worker {
public:
	StopWorker() :
			Worker() {
	}
	void run() {
		_selectedJob->stop();
	}
	Job *_selectedJob;
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
	int waitCursorCounter;
	Glib::Mutex refreshMutex;
	RefreshWorker refreshWorker;
	StartWorker startWorker;
	StopWorker stopWorker;

	void setPosition();
	int loadConfInt(Glib::RefPtr<Gnome::Conf::Client> &gConfClient, const Glib::ustring &windowConfPath,
			const Glib::ustring &keyName);
	bool loadConfBool(Glib::RefPtr<Gnome::Conf::Client> &gConfClient, const Glib::ustring &windowConfPath,
			const Glib::ustring &keyName);
	void initRightPanel();
	void setWaitCursor();
	void unsetWaitCursor();

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
	void on_refresh_complete();
	void on_start_clicked();
	void on_start_complete();
	void on_restart_clicked();
	void on_stop_clicked();
	void on_stop_complete();
	void on_menu_file_exit();
	void on_menu_help_about();
	void on_set_manual_clicked();

	// Overriden
	bool on_configure_event(GdkEventConfigure* event);
	bool on_window_state_event(GdkEventWindowState* event);

};

#endif /* MMWINDOW_H_ */
