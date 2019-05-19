#include "add_tournament_form.h"
#include "./../sql/sql.h"
#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>

/*
* Creates a form that allows a tournament to be added to the database.
* The form contains a textbox to input the tournament name and a
* button to save the tournament name.
* 
* @param container_form the form from which this method is called
*                       this is the tournament list form
* @param tournament_list a listbox containing the name of all of the tournaments
*/
void showAddTournamentForm(form& container_form, listbox& tournament_list) {
    //https://qpcr4vir.github.io/nana-doxy/html/d6/da9/modal_form_8cpp-example.html

    const int FORM_WIDTH = 600;
    const int FORM_HEIGHT = 200;
    form add_tournament_form(container_form, API::make_center(container_form, FORM_WIDTH, FORM_HEIGHT));

    const char* form_layout =
        "vert"
        "<weight = 20% title_label>"
        "<>"
        "<weight = 10%"
            "<weight = 10%>"
            "<weight = 20% name_label>"
            "<weight = 3%>"
            "<name_textbox>"
            "<weight = 10%>"
        ">"
        "<>"
        "<weight = 15%"
            "<>"
            "<weight = 20% save_button>"
            "<>"
        ">"
        "<>";

    add_tournament_form.div(form_layout);
    add_tournament_form.caption("Add Tournament");
    add_tournament_form.bgcolor(colors::white);

    label title_label {add_tournament_form, "<bold color=0xffa500 size=20>Add Tournament</>"};
    title_label.text_align(align::center);
    title_label.format(true);

    label name_label {add_tournament_form};
    name_label.caption("Tournament Name");
    name_label.text_align(align::left, align_v::center);

    textbox name_textbox {add_tournament_form};

    button save_button {add_tournament_form};
    save_button.caption("Save");
    save_button.events().click([&] {
        saveButtonClick(name_textbox, tournament_list);
        add_tournament_form.close();
    });

    add_tournament_form["title_label"] << title_label;
    add_tournament_form["name_label"] << name_label;
    add_tournament_form["name_textbox"] << name_textbox;
    add_tournament_form["save_button"] << save_button;

    add_tournament_form.collocate();
    add_tournament_form.modality();
}

/*
* Called once the save button has been clicked. This method ensures that a valid
* name has been entered and then saves it to the database and appends the
* new tournament to the list of tournaments.
* 
* @param name_textbox the textbox holding the tournament name
@ @param tournament_list the list containing all tournaments
*/
void saveButtonClick(textbox& name_textbox, listbox& tournament_list) {
    Sql sql;
    std::string name = name_textbox.getline(0).value();
    auto category = tournament_list.at(0);

    if(name != "") {
        sql.addTournament(name);
        category.append({name});
    }
}