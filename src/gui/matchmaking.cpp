#include "matchmaking.h"
#include "tournament_details_form.h"
#include <random>

/*
* Generates a match schedule. It determines the minimum number of matches required
* so that all teams play 5 times for a given tournament. Then, each team is assigned
* to 5 different alliance slots R1, R2, B1, B2 for different match numbers.
*
* @param container_form the form from which this method is called, contains the team input textbox
* @param match_list the listbox that contains a list of the tournament's matches
* @param teams a vector containing all teams participating in the tournament
* @param tournament_name the name of the tournament
*/
void generateMatchSchedule(form& container_form, listbox& match_list, std::vector<Team> teams, std::string tournament_name) {
    const int NUM_MATCHES_IN_TOURNAMENT = ceil((double)teams.size() * NUM_MATCHES_PER_TEAM / NUM_TEAMS_PER_MATCH);
    
    Sql sql;
    int tournament_id = sql.getTournamentId(tournament_name);
    std::vector<std::pair<int, int>> match_indices; //Stores (match #, alliance #) pairs
    std::vector<Match> matches;

    int retry_count = 0;
    const int NUM_TRIES = 10;

    while(retry_count < NUM_TRIES) {
        match_indices = populateMatchIndices(NUM_MATCHES_IN_TOURNAMENT);
        matches = std::vector<Match>();

        for(int i = 0; i < NUM_MATCHES_IN_TOURNAMENT; i++) {
            matches.push_back(Match(tournament_id));
        }

        bool successful_assignment = assignTeamsToAlliance(teams, match_indices, matches);
        if(successful_assignment) {
            break; //All teams has been assigned to matches, break out of retry loop
        } else {
            retry_count++;
        }
    }

    //Match generation has been tried the maximum number of times, offer user
    //option to retry again
    if(retry_count == NUM_TRIES) {
        retryMatchGeneration(container_form, match_list, teams, tournament_name);
        return;
    }

    addSurrogates(teams, matches, match_indices);
    sql.deleteMatchesFromTournament(tournament_id); //Any currently existing match list is overwritten
    sql.deleteScoresFromTournament(tournament_id); //Match list is regenerated, so old scores are not valid
    sql.addMatches(matches);
    match_list.erase(); //Remove previous matches from listbox
    populateMatchList(match_list, tournament_name);
    container_form.close();
}

/*
* Loops through all teams and match alliance pairs to assign teams to play in matches.
* 
* @param teams a vector containing all teams competing in the tournaemnt
* @param match_indices a vector of (match number, alliance) pairs that do not have
*                      an assigned team - this will be modified
* @param matches a vector of all matches for the tournament - this will be modified
* @return whether or not the match assignment was successful
*/
bool assignTeamsToAlliance(std::vector<Team> teams, std::vector<std::pair<int, int>>& match_indices, std::vector<Match>& matches) {
    for(Team team : teams) {
        for(int i = 0; i < NUM_MATCHES_PER_TEAM; i++) {
            bool successful_assignment = assignTeamToAlliance(team, match_indices, matches);
            if(!successful_assignment) {
                return false;
            }
        }
    }

    return true;
}

/*
* Assigns a team to their matches. A team is assigned to 5 matches, and the alliance for which they
* play is randomly generated. If a team is assigned to play in a match in which it is already competing,
* the matchmaking will be retried.
* 
* @param team the team that is to be assigned to match/alliance slots
* @param match_indices a vector of (match number, alliance) pairs that do not have
*                      an assigned team - this will be modified
* @param matches a vector of all matches for the tournament - this will be modified
* @return A boolean indicating whether match assignment was successful
*/
bool assignTeamToAlliance(Team team, std::vector<std::pair<int, int>>& match_indices, std::vector<Match>& matches) {
    //https://stackoverflow.com/a/7560564
    std::random_device rand_device;
    std::mt19937 eng {rand_device()};
    std::uniform_int_distribution<> dist(0, match_indices.size() - 1); //RNG is inclusive [min, max]
    const int NUM_TRIES = 5;

    //Match generation may fail if the program chooses matches
    //in which the team is already competing
    int retry_count = 0;

    while(retry_count < NUM_TRIES) {
        int match_alliance_pair_index = dist(eng);
        std::pair<int, int> match_alliance = match_indices[match_alliance_pair_index];

        //Ensure that team is not already competing in the match
        int match_number = match_alliance.first;
        if(matches[match_number].red1 == team.number ||
            matches[match_number].red2 == team.number ||
            matches[match_number].blue1 == team.number ||
            matches[match_number].blue2 == team.number) {
            //Pick different match number by continuing and generating new random match_index
            retry_count++;
            continue;
        }

        matches = setAllianceTeam(matches, match_alliance.first, team.number, match_alliance.second, false);
        match_indices.erase(match_indices.begin() + match_alliance_pair_index);
        break; //Team has been assigned slot, break out of retry loop
    }

    //No valid match list could be generated
    if(retry_count == NUM_TRIES) {
        return false;
    }

    return true;
}

/*
* Creates a vector of match number and alliance pairs.
* 
* @param matches_in_tournament the total number of matches that are to be played in this tournament
* @return a vector of (match number, alliance pairs), where R1, R2, B1, B2 are 0, 1, 2, 3 respectively
*/
std::vector<std::pair<int, int>> populateMatchIndices(int matches_in_tournament) {
    std::vector<std::pair<int, int>> match_indices;

    //Build list of all possible indices: (match number, alliance_index) pair
    for(int i = 0; i < matches_in_tournament; i++) {
        for(int j = 0; j < NUM_TEAMS_PER_MATCH; j++) {
            match_indices.push_back({i, j});
        }
    }

    return match_indices;
}

/*
* If the match generation has failed, the user is prompted to retry. In that case, the generateMatchSchedule method
* is called recursively to try again. Match generation failure can happen if the RNG produces bad values many times,
* or if there are too few teams in a tournament.
* 
* @param container_form the form from which this method is called, contains the team input textbox
* @param match_list the listbox that contains a list of the tournament's matches
* @param teams a vector containing all teams participating in the tournament
* @param tournament_name the name of the tournament
*/
void retryMatchGeneration(form& container_form, listbox& match_list, std::vector<Team> teams, std::string tournament_name) {
    msgbox confirm_retry_messagebox {container_form, "Confirm", msgbox::yes_no};
    confirm_retry_messagebox.icon(msgbox::icon_question);
    confirm_retry_messagebox << "The match generation has failed. Would you like to try again?";
    msgbox::pick_t confirmation_result = confirm_retry_messagebox.show();

    if(confirmation_result == msgbox::pick_yes) {
        //Recursively call generateMatchSchedule to try match generation again
        generateMatchSchedule(container_form, match_list, teams, tournament_name);
    }
}

/*
* Once all teams have been assigned to a match, there may still be a few matches that do not have
* four teams competing. In that case, randomly chosen teams are assigned to compete in an extra
* match in one of the unfilled alliance slots. These matches are surrogate matches, and do not count
* in that team's ranking.
* 
* @param teams teams a vector containing all teams participating in the tournament
* @param matches a list of all matches that is filled with teams, aside from matches that require a surrogate
* @param match_indices a vector of (match number, alliance) pairs that do not have
*                      an assigned team
*/
void addSurrogates(std::vector<Team> teams, std::vector<Match>& matches, std::vector<std::pair<int, int>> match_indices) {
    std::random_device rand_device;
    std::mt19937 eng {rand_device()};
    
    for(int i = 0; i < match_indices.size(); i++) {
        std::uniform_int_distribution<> dist(0, teams.size() - 1); //RNG is inclusive [min, max]
        Team team = teams[dist(eng)];
        matches = setAllianceTeam(matches, match_indices[i].first, team.number, match_indices[i].second, true);
    }
}

/*
* Sets the match indicated by match_number to a particular team. The alliance to which they are assigned
* is indicated by the alliance_index, with 0 = R1, 1 = R2, 2 = B1, and 3 = B2. The existence of a surrogate
* match is also added. Matches is returned, and so is not passed by reference.
* 
* @param matches a vector of matches for the tournament
* @param match_number the match number to which the team is being assigned
* @param team_number the number of the team being assigned
* @param alliance_index the alliance index to which the team is being assigned (R1, R2, B1, B2) = (0, 1, 2, 3)
* @param surrogate whether or not this match is a surrogate match for the team
*/
std::vector<Match> setAllianceTeam(std::vector<Match> matches, int match_number, int team_number, int alliance_index, bool surrogate) {
    if(alliance_index == 0) {
        matches[match_number].red1 = team_number;
        matches[match_number].red1_surrogate = surrogate;
    } else if(alliance_index == 1) {
        matches[match_number].red2 = team_number;
        matches[match_number].red2_surrogate = surrogate;
    } else if(alliance_index == 2) {
        matches[match_number].blue1 = team_number;
        matches[match_number].blue1_surrogate = surrogate;
    } else {
        matches[match_number].blue2 = team_number;
        matches[match_number].blue2_surrogate = surrogate;
    }

    return matches;
}