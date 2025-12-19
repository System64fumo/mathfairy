#include "window.hpp"
#include <gtkmm/cssprovider.h>
#include <gtkmm/adjustment.h>
#include <gtkmm/button.h>

mathfairy::mathfairy() : 
	box_main(Gtk::Orientation::VERTICAL) {
	set_title("Math Fairy");
	set_default_size(300, 560);
	set_child(box_main);

	scrolled_history.set_child(label_history);
	scrolled_history.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
	scrolled_history.set_vexpand(true);
	scrolled_history.set_hexpand(true);
	scrolled_history.set_margin(5);
	scrolled_history.set_margin_bottom(0);

	label_history.add_css_class("title-4");
	label_history.set_xalign(1.0);
	label_history.set_yalign(1.0);
	label_history.set_sensitive(false);
	label_history.set_justify(Gtk::Justification::RIGHT);

	entry_result.add_css_class("flat");
	entry_result.add_css_class("title-1");
	entry_result.set_placeholder_text("0");
	entry_result.set_alignment(1);
	entry_result.set_margin(5);
	entry_result.set_margin_top(0);
	entry_result.set_input_purpose(Gtk::InputPurpose::NUMBER);
	entry_result.signal_activate().connect(
		std::bind(&mathfairy::on_button_clicked, this, "=")
	);

	grid_numpad.set_row_homogeneous(true);
	grid_numpad.set_column_homogeneous(true);
	grid_numpad.set_row_spacing(5);
	grid_numpad.set_column_spacing(5);
	grid_numpad.set_margin(5);
	grid_numpad.set_vexpand(true);

	box_main.append(scrolled_history);
	box_main.append(entry_result);
	box_main.append(grid_numpad);

	scrolled_history.set_vexpand(true);
	scrolled_history.set_propagate_natural_width(true);

	struct ButtonInfo {
		std::string label;
		unsigned char col;
		unsigned char row;
		unsigned char width;
		unsigned char height;
	};

	const ButtonInfo buttons[] = {
		// Row 0
		{"c", 0, 0, 1, 1},
		{"(", 1, 0, 1, 1},
		{")", 2, 0, 1, 1},
		{"/", 3, 0, 1, 1},
		
		// Row 1
		{"7", 0, 1, 1, 1},
		{"8", 1, 1, 1, 1},
		{"9", 2, 1, 1, 1},
		{"x", 3, 1, 1, 1},
		
		// Row 2
		{"4", 0, 2, 1, 1},
		{"5", 1, 2, 1, 1},
		{"6", 2, 2, 1, 1},
		{"-", 3, 2, 1, 1},
		
		// Row 3
		{"1", 0, 3, 1, 1},
		{"2", 1, 3, 1, 1},
		{"3", 2, 3, 1, 1},
		{"+", 3, 3, 1, 1},
		
		// Row 4
		{"0", 0, 4, 2, 1},
		{".", 2, 4, 1, 1},
		{"=", 3, 4, 1, 1}
	};

	for (const auto& btnInfo : buttons) {
		Gtk::Button* button = Gtk::make_managed<Gtk::Button>(btnInfo.label);
		grid_numpad.attach(*button, btnInfo.col, btnInfo.row, btnInfo.width, btnInfo.height);
		button->set_focusable(false);
		button->signal_clicked().connect([&, label = btnInfo.label]() {
			on_button_clicked(label);
		});
	}

	Glib::RefPtr<Gtk::CssProvider> css = Gtk::CssProvider::create();
	css->load_from_data(
		"entry {"
		"	border: unset;"
		"	outline: unset;"
		"}"
	);
	get_style_context()->add_provider_for_display(
		property_display(),
		css,
		GTK_STYLE_PROVIDER_PRIORITY_USER
	);
	entry_result.grab_focus();
}

void mathfairy::update_history(const std::string& expression, const std::string& result) {
	std::string current_text = label_history.get_text();

	std::string prefix = current_text.empty() ? "" : "\n";
	std::string new_entry = prefix + expression + " = " + result;

	std::string updated_text = current_text + new_entry;

	int line_count = 0;
	size_t pos = 0;
	while (pos != std::string::npos && line_count < 50) {
		pos = updated_text.find('\n', pos + 1);
		line_count++;
	}

	if (pos != std::string::npos) {
		updated_text = updated_text.substr(0, pos);
	}

	label_history.set_text(updated_text);
}

void mathfairy::on_button_clicked(const std::string& btn) {
	if (btn == "c") {
		if (entry_result.get_text().length() > 1)
			entry_result.set_text(entry_result.get_text().substr(0, entry_result.get_text().length() - 1));
		else {
			entry_result.set_text("");
			label_history.set_text("");
		}
	}
	else if (btn == "=") {
		std::string expression = entry_result.get_text();
		if (expression.empty()) {
			return;
		}

		has_error = false;
		
		double result = evaluate_expression(expression);
		
		if (has_error) {
			entry_result.set_text("");
			entry_result.set_placeholder_text("Error");
			has_error = false;
		}
		else {
			std::string formatted_result = format_double(result);
			entry_result.set_text(formatted_result);
			entry_result.set_placeholder_text("0");

			update_history(expression, formatted_result);
			auto vadj = scrolled_history.get_vadjustment();
			vadj->set_value(vadj->get_upper());
		}
	}
	else {
		entry_result.set_text(entry_result.get_text() + btn);
	}
	entry_result.set_position(entry_result.get_text().length());
}