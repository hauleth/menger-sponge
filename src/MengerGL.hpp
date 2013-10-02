/**
 * MengerGL.hpp
 *
 *  Created on: 2010-01-25
 *      Author: hauleth
 */

#ifndef MENGERGL_HPP_
#define MENGERGL_HPP_

#include <gtkmm.h>
#include <gtkglmm.h>

//class MengerGL: public Gtk::DrawingArea, public Gtk::GL::Widget<MengerGL> {
class MengerGL: public Gtk::GL::DrawingArea {
private:
	int list;
	double rot_x, rot_y, rot_z, l;
	bool full;
	int depth;
	/// function drawing Menger Sponge
	void draw_menger(uint, GLdouble, GLdouble, GLdouble, double, Glib::RefPtr<
			Gdk::GL::Drawable>&);
public:
	MengerGL(int = 4, bool = true);
	virtual ~MengerGL() {
	}
protected:
	/// main draw
	void on_realize();
	bool on_configure_event(GdkEventConfigure*);
	/// on show
	bool on_expose_event(GdkEventExpose*);
	bool on_idle();
public:
	/// Update window
	void update() {
		this->get_window()->process_updates(false);
	}
	/// Invalidate whole window.
	void invalidate() {
		get_window()->invalidate_rect(get_allocation(), false);
	}

	void set_rotate(double x, double y, double z) {
		rot_x = x;
		rot_y = y;
		rot_z = z;
	}
	void get_rotate(double& x, double& y, double& z) {
		x = rot_x;
		y = rot_y;
		z = rot_z;
	}

	void zoom(double z) {
		l = 0.0;
		l = z;
	}
};

#endif /* MENGERGL_HPP_ */
