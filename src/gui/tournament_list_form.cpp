#include "tournament_list_form.h"
#include "add_tournament_form.h"
#include "tournament_details_form.h"
#include <nana/gui/widgets/textbox.hpp>

/*
* Creates a form that displays the list of all tournaments. This form 
* displays a list of all currently existing tournaments and 
* allows a user to add a tournament to the database.
* This form is the main entry into the GUI.
*/
void showTournamentListForm() {
    //http://nanapro.org/en-us/forum/index.php?u=/topic/91/ggmaximaze-form
    form main_screen(rectangle {screen().desktop_size()});
    API::fullscreen(main_screen, true);
    main_screen.bgcolor(colors::white);

    const char* main_screen_layout = 
        "vert "
        "<weight = 10% title_heading>"
        "<" //Row with tournament list - height auto sized
            "<weight = 15%>"
            "<tournament_list>"
            "<weight = 15%>"
        ">"
        "<weight = 5%>" //Space between tournaments and buttons
        "<weight = 5%" //Row containing buttons
            "<weight = 25%>"
            "<"
                "<add_tournament>"
                "<weight = 10%>"
                "<edit_tournament>"
                "<weight = 10%>"
                "<delete_tournament>"
            ">"
            "<weight = 25%>"
        ">"
        "<weight = 15%>"; //Padding at bottom
    main_screen.div(main_screen_layout);

    label title_text {main_screen, "<bold color=0xffa500 size=30>FIRST Tech Challenge Rover Ruckus Scoring</>"};
    title_text.format(true);
    title_text.text_align(align::center);

    listbox tournament_list {main_screen};
    tournament_list.enable_single(true, true); //Prevents the user from selecting multiple rows
    tournament_list.sortable(false);
    populateTournaments(tournament_list);

    std::string selected_tournament = "";
    tournament_list.events().selected([&](const arg_listbox& selected) {
        selected_tournament = selected.item.text(0);
    });

    button edit_tournament {main_screen};
    edit_tournament.caption("Select");
    edit_tournament.events().click([&] {
        if(selected_tournament != "") {
            showTournamentDetailsForm(main_screen, selected_tournament);
        }
    });

    button add_tournament {main_screen};
    add_tournament.caption("Add");
    add_tournament.events().click([&] {
        showAddTournamentForm(main_screen, tournament_list);
    });

    button delete_tournament {main_screen};
    delete_tournament.caption("Delete");
    delete_tournament.events().click([&] {
        deleteTournamentClick(selected_tournament, tournament_list);
    });

    main_screen["title_heading"] << title_text;
    main_screen["tournament_list"] << tournament_list;
    main_screen["edit_tournament"] << edit_tournament;
    main_screen["add_tournament"] << add_tournament;
    main_screen["delete_tournament"] << delete_tournament;

    main_screen.collocate();
    main_screen.show();
    exec();
}

/*
* Retrieves a list of tournaments from the database and adds it to the 
* tournament_list listbox.
* 
* @param tournament_list the listbox to which tournaments will be added
*/
void populateTournaments(listbox& tournament_list) {
    const int COLUMN_WIDTH = 887;
    Sql sql;
    std::vector<Tournament> tournaments = sql.getTournaments("");

    if(tournament_list.column_size() == 0) {
        tournament_list.append_header("Tournament Name");
    }

    tournament_list.column_at(0).width(COLUMN_WIDTH);
    auto category = tournament_list.at(0);

    for(Tournament tournament : tournaments) {
        category.append({tournament.name});
    }
}

/*
* Called when the delete tournament button is clicked. This method
* ensures that a valid tournament was selected and removes it from both
* the database and the list of tournaments. All associated scores
* and matches are likewise deleted.
* 
* @param selected_tournament the name of the tournament that is to be deleted
* @param tournament_list the list holding the names of all the tournaments
*/
void deleteTournamentClick(std::string tournament_name, listbox& tournament_list) {
    if(tournament_name != "") {
        Sql sql;
        int tournament_id = sql.getTournamentId(tournament_name);

        sql.deleteTournament(tournament_name);
        sql.deleteMatchesFromTournament(tournament_id);
        sql.deleteScoresFromTournament(tournament_id);
        tournament_list.clear();
        populateTournaments(tournament_list);
    }
}