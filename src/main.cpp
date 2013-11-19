#include <iostream>
#include <sstream>
#include <cstdlib>

#include <gtkmm.h>
#include <gtkglmm.h>

#include "MengerGL.hpp"

template <typename Draw>
class App: public Gtk::Window {
private:
  Draw sponge;
  Gtk::VBox vbox;
  Gtk::Button quit;
protected:
  void on_button_click() {
    Gtk::Main::quit();
  }
  bool on_key_press_event(GdkEventKey* ev) {
    double x, y, z;
    sponge.get_rotate(x, y, z);
    double l = 0.0;

    switch (ev->keyval) {
    case GDK_Up:
      x += 5.0;
      break;
    case GDK_Down:
      x -= 5.0;
      break;
    case GDK_Left:
      y += 5.0;
      break;
    case GDK_Right:
      y -= 5.0;
      break;
    case GDK_z:
      z += 5.0;
      break;
    case GDK_x:
      z -= 5.0;
      break;
    case GDK_q:
      l -= 2.0;
      break;
    case GDK_w:
      l += 2.0;
      break;
    case GDK_Escape:
      Gtk::Main::quit();
      break;
    default:
      return true;
    }

    sponge.set_rotate(x, y, z);
    sponge.zoom(l);
    sponge.invalidate();

    return true;
  }
public:
  App() : sponge(3), vbox(false, 0), quit("Wyjście") {
    int major, minor;
    std::stringstream str;
    if (Gdk::GL::query_version(major, minor))
    {
      str << "(OpenGL " << major << '.' << minor << ')';
    }
    set_title("Kostka Mengera " + str.str());

    set_reallocate_redraws(true);
    add(vbox);

    sponge.set_size_request(800, 600);
    vbox.pack_start(sponge);

    quit.signal_clicked().connect(
        sigc::mem_fun(*this, &App::on_button_click));

    vbox.pack_start(quit, Gtk::PACK_SHRINK, 0);

    show_all();
  }
};

int main(int argc, char** argv) {
  Gtk::Main m(argc, argv);
  Gdk::GL::init(argc, argv);

  App<MengerGL> a;
  m.run(a);

  return 0;
}
