#include "tournament_details_form.h"

#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include "./../sql/sql.h"
#include "add_teams_form.h"
#include "add_scores_form.h"
#include "view_rankings_form.h"

/*
* Creates a form that contains the details for a tournament. This form
* shows the match list and allows users to add teams that are competing.
* @param container_form the form from which this method is called
*                       this form is the tournament list form
* @param tournament_name the name of the tournament in which teams are competing
*/
void showTournamentDetailsForm(form& container_form, std::string tournament_name) {
    form tournament_details_form(rectangle {screen().desktop_size()});
    API::fullscreen(container_form, true);

    const char* form_layout =
        "vert"
        "<weight = 10% title_label>"
        "<weight = 5%" //Row containing buttons
            "<"
                "<>"
                "<weight = 20% edit_teams_button>"
                "<>"
                "<weight = 20% edit_scores_button>"
                "<>"
                "<weight = 20% view_rankings_button>"
                "<>"
            ">"
        ">"
        "<weight = 5%>"
        "<"
            "<weight = 25%>"
            "<matches_listbox>"
            "<weight = 25%>"
        ">"
        "<weight = 5% margin=[20] surrogate_label>"
        "<weight = 15%>";

    tournament_details_form.div(form_layout);
    tournament_details_form.caption(tournament_name);
    tournament_details_form.bgcolor(colors::white);

    listbox match_list {tournament_details_form};
    match_list.enable_single(true, true); //Prevents the user from selecting multiple rows
    match_list.sortable(false);
    populateMatchList(match_list, tournament_name);

    std::string selected_match = "";
    match_list.events().selected([&](const arg_listbox& selected) {
        selected_match = selected.item.text(0);
    });

    label title_label {tournament_details_form, "<bold color=0xffa500 size=20>" + tournament_name + "</>"};
    title_label.text_align(nana::align::center);
    title_label.format(true);

    button edit_teams_button {tournament_details_form};
    edit_teams_button.caption("Edit Teams");
    edit_teams_button.events().click([&] {
        showAddTeamsForm(tournament_details_form, match_list, tournament_name);
    });

    button edit_scores_button {tournament_details_form};
    edit_scores_button.caption("Edit Scores");
    edit_scores_button.events().click([&] {
        showAddScoresForm(tournament_details_form, tournament_name, selected_match);
    });

    button view_rankings_button {tournament_details_form};
    view_rankings_button.caption("View Team Rankings");
    view_rankings_button.events().click([&] {
        showRankingsForm(tournament_details_form, tournament_name);
    });

    label surrogate_label {tournament_details_form};
    surrogate_label.caption("Note: * indicates a surrogate match. These matches do NOT count for ranking.");
    surrogate_label.text_align(align::center);

    tournament_details_form["title_label"] << title_label;
    tournament_details_form["edit_teams_button"] << edit_teams_button;
    tournament_details_form["edit_scores_button"] << edit_scores_button;
    tournament_details_form["view_rankings_button"] << view_rankings_button;
    tournament_details_form["matches_listbox"] << match_list;
    tournament_details_form["surrogate_label"] << surrogate_label;

    tournament_details_form.collocate();
    tournament_details_form.modality();
}

/*
* Retrieves a list of all matches for a particular tournament and adds
* this list of matches to the matches listbox.
* Any surrogate matches are indicated by an asterisk (*)
* If the match_list listbox already has headings, new headings
* will not be appended.
* 
* @param match_list the listbox to which matches will be added
* @param tournament_name the name of the tournament from which the
*                        match lists will be retrieved
*/
void populateMatchList(listbox& match_list, std::string tournament_name) {
    const int MATCH_NUMBER_COLUMN_WIDTH = 119;
    const int TEAM_NUMBER_COLUMN_WIDTH = 128;

    //If this method is called on an existing listbox with existing headers
    //the headers should not be appended
    if(match_list.column_size() == 0) {
        match_list.append_header("Match Number");
        match_list.append_header("Red 1");
        match_list.append_header("Red 2");
        match_list.append_header("Blue 1");
        match_list.append_header("Blue 2");
    }

    listbox::cat_proxy category = match_list.at(0);
    Sql sql;
    std::string tournament_id = std::to_string(sql.getTournamentId(tournament_name));
    std::vector<Match> matches = sql.getMatches("WHERE tournament_id = " + tournament_id);

    for(int i = 1; i < matches.size() + 1; i++) { //Start at 1 to use as match index
        //Surrogate matches have * next to the team number
        std::string red1 = std::to_string(matches[i - 1].red1) + (matches[i - 1].red1_surrogate ? " *" : "");
        std::string red2 = std::to_string(matches[i - 1].red2) + (matches[i - 1].red2_surrogate ? " *" : "");
        std::string blue1 = std::to_string(matches[i - 1].blue1) + (matches[i - 1].blue1_surrogate ? " *" : "");
        std::string blue2 = std::to_string(matches[i - 1].blue2) + (matches[i - 1].blue2_surrogate ? " *" : "");
        category.append({std::to_string(i), red1, red2, blue1, blue2});
    }

    for(int i = 0; i < match_list.column_size(); i++) {
        match_list.column_at(i).width(i == 0 ? MATCH_NUMBER_COLUMN_WIDTH : TEAM_NUMBER_COLUMN_WIDTH);
        match_list.column_at(i).text_align(align::center);
    }

    match_list.enable_single(true, true); //Stops users from selecting multiple rows
}