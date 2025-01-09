#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/grid.h>
#include <gtkmm/entry.h>

// TODO: This shouldn't be here
double evaluate_expression(const std::string& expression);

class mathfairy : public Gtk::Window {
	public:
		mathfairy();

	private:
		Gtk::Entry entry_result;
		Gtk::Box box_main;
		Gtk::Grid grid_numpad;

		void on_button_clicked(const std::string& btn);
};
