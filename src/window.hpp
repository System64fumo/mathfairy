#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/grid.h>
#include <gtkmm/entry.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/label.h>

// TODO: This shouldn't be here
extern bool has_error;
double evaluate_expression(const std::string& expression);
std::string format_double(double value);

class mathfairy : public Gtk::Window {
	public:
		mathfairy();

	private:
		Gtk::Box box_main;
		Gtk::ScrolledWindow scrolled_history;
		Gtk::Label label_history;
		Gtk::Entry entry_result;
		Gtk::Grid grid_numpad;

		void on_button_clicked(const std::string& btn);
		void update_history(const std::string& expression, const std::string& result);
};
