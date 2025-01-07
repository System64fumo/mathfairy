#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/grid.h>
#include <gtkmm/editablelabel.h>

// TODO: This shouldn't be here
double evaluate_expression(const std::string& expression);

class mathfairy : public Gtk::Window {
	public:
		mathfairy();

	private:
		Gtk::EditableLabel editablelabel_main;
		Gtk::Box box_main;
		Gtk::Grid grid_numpad;
};
