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

class mmWindow: public Gtk::Window {

public:
	mmWindow();
	bool on_configure_event(GdkEventConfigure* event);
	bool on_window_state_event(GdkEventWindowState* event);

	void savePosition(const Glib::ustring &windowConfPath);
	void loadPosition(const Glib::ustring &windowConfPath);
private:
	bool isMaximized;
	int size_width, size_height, pos_x, pos_y;
	bool positionValid;

	void setPosition();
	int loadConfInt(Glib::RefPtr<Gnome::Conf::Client> &gConfClient, const Glib::ustring &windowConfPath,
			const Glib::ustring &keyName);
	bool loadConfBool(Glib::RefPtr<Gnome::Conf::Client> &gConfClient, const Glib::ustring &windowConfPath,
			const Glib::ustring &keyName);
};

#endif /* MMWINDOW_H_ */
