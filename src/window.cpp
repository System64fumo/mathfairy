#include "window.hpp"
#include <gtkmm/cssprovider.h>
#include <gtkmm/button.h>

mathfairy::mathfairy() : box_main(Gtk::Orientation::VERTICAL) {
	set_child(box_main);
	box_main.append(editablelabel_main);
	box_main.append(grid_numpad);

	// TODO: Add history section

	// Result label
	editablelabel_main.get_style_context()->add_class("result");
	editablelabel_main.set_text("0");
	editablelabel_main.set_vexpand(true);
	editablelabel_main.set_alignment(1);
	editablelabel_main.set_valign(Gtk::Align::END);

	// Grid
	grid_numpad.set_row_homogeneous(true);
	grid_numpad.set_column_homogeneous(true);
	grid_numpad.set_row_spacing(5);
	grid_numpad.set_column_spacing(5);
	grid_numpad.set_vexpand(true);

	// TODO: Add last operation function (Press equals to redo the last operation if none is provided)
	// TODO: Pretty sure there's a better way to do this

	// Buttons
	Gtk::Button* sign_delete = Gtk::make_managed<Gtk::Button>("c");
	grid_numpad.attach(*sign_delete, 0, 0, 1, 1);
	Gtk::Button* sign_br_strt = Gtk::make_managed<Gtk::Button>("(");
	grid_numpad.attach(*sign_br_strt, 1, 0, 1, 1);
	Gtk::Button* sign_br_end = Gtk::make_managed<Gtk::Button>(")");
	grid_numpad.attach(*sign_br_end, 2, 0, 1, 1);

	Gtk::Button* num_0 = Gtk::make_managed<Gtk::Button>("0");
	grid_numpad.attach(*num_0, 0, 4, 2, 1);
	Gtk::Button* num_1 = Gtk::make_managed<Gtk::Button>("1");
	grid_numpad.attach(*num_1, 0, 3, 1, 1);
	Gtk::Button* num_2 = Gtk::make_managed<Gtk::Button>("2");
	grid_numpad.attach(*num_2, 1, 3, 1, 1);
	Gtk::Button* num_3 = Gtk::make_managed<Gtk::Button>("3");
	grid_numpad.attach(*num_3, 2, 3, 1, 1);
	Gtk::Button* num_4 = Gtk::make_managed<Gtk::Button>("4");
	grid_numpad.attach(*num_4, 0, 2, 1, 1);
	Gtk::Button* num_5 = Gtk::make_managed<Gtk::Button>("5");
	grid_numpad.attach(*num_5, 1, 2, 1, 1);
	Gtk::Button* num_6 = Gtk::make_managed<Gtk::Button>("6");
	grid_numpad.attach(*num_6, 2, 2, 1, 1);
	Gtk::Button* num_7 = Gtk::make_managed<Gtk::Button>("7");
	grid_numpad.attach(*num_7, 0, 1, 1, 1);
	Gtk::Button* num_8 = Gtk::make_managed<Gtk::Button>("8");
	grid_numpad.attach(*num_8, 1, 1, 1, 1);
	Gtk::Button* num_9 = Gtk::make_managed<Gtk::Button>("9");
	grid_numpad.attach(*num_9, 2, 1, 1, 1);

	Gtk::Button* sign_divide = Gtk::make_managed<Gtk::Button>("/");
	grid_numpad.attach(*sign_divide, 3, 0, 1, 1);
	Gtk::Button* sign_multiply = Gtk::make_managed<Gtk::Button>("x");
	grid_numpad.attach(*sign_multiply, 3, 1, 1, 1);
	Gtk::Button* sign_subtract = Gtk::make_managed<Gtk::Button>("-");
	grid_numpad.attach(*sign_subtract, 3, 2, 1, 1);
	Gtk::Button* sign_add = Gtk::make_managed<Gtk::Button>("+");
	grid_numpad.attach(*sign_add, 3, 3, 1, 1);
	Gtk::Button* sign_equals = Gtk::make_managed<Gtk::Button>("=");
	grid_numpad.attach(*sign_equals, 3, 4, 1, 1);
	Gtk::Button* sign_dot = Gtk::make_managed<Gtk::Button>(".");
	grid_numpad.attach(*sign_dot, 2, 4, 1, 1);

	for (auto& child : grid_numpad.get_children()) {
		auto* button = dynamic_cast<Gtk::Button*>(child);
		button->signal_clicked().connect([&, button]() {
			if (button->get_label() == "c") {
				if (editablelabel_main.get_text().length() > 1)
					editablelabel_main.set_text(editablelabel_main.get_text().substr(0, editablelabel_main.get_text().length() - 1));
				else
					editablelabel_main.set_text("0");
			}
			else if (button->get_label() == "=") {
				try {
					double result = evaluate_expression(editablelabel_main.get_text());
					if (std::floor(result) == result)
						editablelabel_main.set_text(std::to_string(static_cast<int>(result)));
					else
						editablelabel_main.set_text(std::to_string(result));
				}
				catch (const std::exception& e) {
					editablelabel_main.set_text("Error");
					std::printf("%s\n", e.what());
				}

			}
			else {
				if (editablelabel_main.get_text() == "0")
					editablelabel_main.set_text("");
				editablelabel_main.set_text(editablelabel_main.get_text() + button->get_label());
			}
		});
	}

	// Internal styling
	Glib::RefPtr<Gtk::CssProvider> css = Gtk::CssProvider::create();
	css->load_from_data(
		".result {"
		"	font-size: 32px;"
		"	margin: 10px;"
		"}"
		"grid {"
		"	margin: 5px;"
		"}"
	);
	get_style_context()->add_provider_for_display(
		property_display(),
		css,
		GTK_STYLE_PROVIDER_PRIORITY_USER
	);
}
