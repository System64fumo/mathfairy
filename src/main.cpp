#include <gtkmm/application.h>
#include "window.hpp"

int main(int argc, char* argv[]) {
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create("funky.sys64.mathfairy");

	return app->make_window_and_run<mathfairy>(0, nullptr);
}
