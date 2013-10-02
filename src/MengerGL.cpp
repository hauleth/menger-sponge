/**
 * MengerGL.cpp
 *
 *  Created on: 2010-01-25
 *      Author: hauleth
 */

#include "MengerGL.hpp"

#include <iostream>
#include <omp.h>

#include <GL/gl.h>
#include <GL/glu.h>

MengerGL::MengerGL(int depth, bool full) :
	rot_x(20.0), rot_y(30.0), rot_z(0.0), l(0.0), full(full), depth(depth) {
	Glib::RefPtr<Gdk::GL::Config> glconf = Gdk::GL::Config::create(
			Gdk::GL::MODE_RGBA | Gdk::GL::MODE_DEPTH | Gdk::GL::MODE_DOUBLE);
	if(!glconf) {
		std::cerr << "Cannot run OpenGL\n";
		std::exit(1);
	}
	this->set_gl_capability(glconf);
	add_events(Gdk::VISIBILITY_NOTIFY_MASK);
}

void MengerGL::draw_menger(uint lvl, GLdouble x, GLdouble y, GLdouble z,
		double size, Glib::RefPtr<Gdk::GL::Drawable>& gld) {
	if(lvl == 0) {
		glTranslated(x, y, z); // set begin of axes in (x,y,z)
		gld->draw_cube(full, size); // draw a cube
		glTranslated(-x, -y, -z); // return to previous settings
		return;
	}
#define FOR(x) for(int x = -1; x < 2; x++)
	double nsize = size / 3.0;
	FOR(i)
		FOR(j)
#pragma omp parallel for
			FOR(k)
			if(((i * i + j * j) * (i * i + k * k) * (j * j + k * k)) > 0)
					draw_menger(lvl - 1, x + i * nsize, y + j * nsize, z + k
							* nsize, nsize, gld); // call function again for each of small cubes
#undef FOR
}

void MengerGL::on_realize() {
	Gtk::DrawingArea::on_realize(); // call base on_realize()
	Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable(); // get window handle

	// OpenGL begin
	if(!gldrawable->gl_begin(get_gl_context()))
		return;
	// settings of light source
	static GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	static GLfloat light_position[] = { 5.0, 5.0, 0.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);

	list = glGenLists(1);
	glNewList(list, GL_COMPILE);
	glColor3d(0.3, 0.5, 1.0);
	draw_menger(depth, 0.0, 0.0, 0.0, 10.0, gldrawable); // draw Menger Sponge
	glEndList();

	glEnable(GL_NORMALIZE);

	gldrawable->gl_end();
	// OpenGL END
}

bool MengerGL::on_configure_event(GdkEventConfigure* event) {
	Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable(); // get window handle

	// OpenGL BEGIN
	if(!gldrawable->gl_begin(get_gl_context()))
		return false;

	glViewport(0, 0, get_width(), get_height());
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, get_width() * 1.0 / get_height(), 1.0, 60.0); // set perspective
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
//	gluLookAt(0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); // set camera
	glTranslated(0.0, 0.0, -20.0);

	gldrawable->gl_end();
	// OpenGL END

	return true;
}

bool MengerGL::on_expose_event(GdkEventExpose* event) {
	Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable(); // get window handle

	// OpenGL BEGIN
	if(!gldrawable->gl_begin(get_gl_context()))
		return false;
	glTranslated(0.0, 0.0, -l);
	l = 0.0;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glRotated(rot_x, 1.0, 0.0, 0.0);
	glRotated(rot_y, 0.0, 1.0, 0.0);
	glRotated(rot_z, 0.0, 0.0, 1.0);
	glCallList(list);
	glPopMatrix();

	// Swap buffers.
	if(gldrawable->is_double_buffered())
		gldrawable->swap_buffers();
	else
		glFlush();

	gldrawable->gl_end();
	// OpenGL END

	return true;
}

bool MengerGL::on_idle() {
	invalidate();
	update();
	return true;
}
