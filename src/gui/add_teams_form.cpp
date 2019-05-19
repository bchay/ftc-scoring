#include "add_teams_form.h"
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include "matchmaking.h"

/*
* Creates a form that allows users to select teams that are competing in a tournament.
* The form contains a textbox to enter team information and a save button.
* Team information should be entered in the format "number name".
* 
* @param container_form the form from which this method is called
*                       this is the touranment details form
* @param match_list the listbox containing a list of matches for the tournament
* @param tournament_name the name of the tournament to which teams are to be added
*/
void showAddTeamsForm(form& container_form, listbox& match_list, std::string tournament_name) {
    form add_teams_form(rectangle {screen().desktop_size()});
    API::fullscreen(container_form, true);

    const char* form_layout =
        "vert"
        "<weight = 7.5% title_label>"
        "<weight = 5% instruction_label>"
        "<"
            "<weight = 20%>"
            "<teams_textbox>"
            "<weight = 20%>"
        ">"
        "<weight = 5%>"
        "<weight = 5%"
            "<>"
            "<weight = 10% save_button>"
            "<>"
        ">"
        "<weight = 15%>";

    add_teams_form.div(form_layout);
    add_teams_form.caption("Add Teams");
    add_teams_form.bgcolor(colors::white);

    label title_label {add_teams_form, "<bold color=0xffa500 size=20>Add Teams</>"};
    title_label.text_align(nana::align::center);
    title_label.format(true);

    label instruction_label {add_teams_form};
    instruction_label.text_align(nana::align::center);
    instruction_label.caption("On each line, enter the team number followed by a space and then the team name.");

    textbox teams_textbox {add_teams_form};
    populateTeamList(teams_textbox, tournament_name);

    button save_button {add_teams_form};
    save_button.caption("Save");
    save_button.events().click([&] {
        //Display confirmation to ensure user wants to erase current matches
        std::string prompt = "Are you sure you want to save the list of teams? Doing so will generate a match list and rewrite any current matches.";
        
        if(showConfirmationPopup(add_teams_form, prompt) == msgbox::pick_yes) {
            saveTeamList(add_teams_form, teams_textbox, match_list, tournament_name);
            //add_teams_form will be closed by generateMatchSchedule if match
            //generation was successful
        }
    });

    add_teams_form["title_label"] << title_label;
    add_teams_form["instruction_label"] << instruction_label;
    add_teams_form["teams_textbox"] << teams_textbox;
    add_teams_form["save_button"] << save_button;

    add_teams_form.collocate();
    add_teams_form.modality();
}

/*
* Retrieves the list of teams that are competing in a tournament from the
* database and puts the information into the team_textbox.
* 
* @param team_textbox the textbox that contains the user-editable list of teams
*                     in the format "number name"
* @param tournament_name the name of the tournament in which teams are competing
*/
void populateTeamList(textbox& team_textbox, std::string tournament_name) {
    Sql sql;
    std::vector<Team> teams = sql.getTeamsInTournament(tournament_name);

    for(Team team : teams) {
        team_textbox.append(std::to_string(team.number) + " " + team.name + "\n", false);
    }
}

/*
* Saves the list of teams from team_textbox to the database. The team number is a unique
* key field, and so cannot be overridden with new data. Thus, a team whose name and number
* are already saved in the database will not have their name overridden if the user only
* enters the team number.
* Upon saving the teams, this method calls generateMatchSchedule.
* 
* @param container_form the form from which this method is called - this form
*                       is the add_teams_form with the team list textbox
* @param teams_textbox the textbox containing the user-entered list of team numbers and names
* @param match_list the listbox that contains matches for the tournament.
*                   Any already existing matches will be overridden.
* @param tournament_name the name of the tournament (Matches 
*/
void saveTeamList(form& container_form, textbox& teams_textbox, listbox& match_list, std::string tournament_name) {
    int i = 0;
    std::vector<Team> teams;
    Sql sql;

    while(teams_textbox.getline(i)) {
        std::string line = teams_textbox.getline(i).value();
        int space_position = line.find(" ");
        
        if(line == "") {
            i++;
            continue;
        }

        //If there is no space, only the number was entered
        //In this case, add the number and an empty string to DB
        if(space_position == std::string::npos) {
            space_position = line.size();
        }

        std::string number = line.substr(0, space_position);
        std::string team_name = line.substr(space_position, line.size());
        int team_number;

        try { //Ensure that the line starts with a team number
            team_number = std::stoi(number);
        } catch(std::invalid_argument e) {
            i++;
            continue;
        }

        Team team(team_number, team_name);
        teams.push_back(team);
        i++;
    }

    sql.addTeams(teams);
    generateMatchSchedule(container_form, match_list, teams, tournament_name);
}

bool showConfirmationPopup(form& container_form, std::string prompt) {
    msgbox confirm_messagebox {container_form, "Confirm", msgbox::yes_no};
    confirm_messagebox.icon(msgbox::icon_question);
    confirm_messagebox << prompt;
    msgbox::pick_t confirmation_result = confirm_messagebox.show();

    return confirmation_result == msgbox::pick_yes;
}