#include "view_rankings_form.h"
#include <nana/gui/widgets/label.hpp>
#include "./../sql/sql.h"

/*
* Creates a form that contains the details for a tournament. This form
* shows the match list and allows users to add teams that are competing.
* 
* @param container_form the form from which this method is called
*                       this form is the tournament list form
* @param tournament_name the name of the tournament in which teams are competing
*/
void showRankingsForm(form& container_form, std::string tournament_name) {
    form view_rankings_form(rectangle {screen().desktop_size()});
    API::fullscreen(container_form, true);

    const char* form_layout =
        "vert"
        "<weight = 10% title_label>"
        "<weight = 70%"
            "<>"
            "<weight = 70% rankings_listbox>"
            "<>"
        ">"
        "<>";

    view_rankings_form.div(form_layout);
    view_rankings_form.caption(tournament_name + " Rankings");
    view_rankings_form.bgcolor(colors::white);

    label title_label {view_rankings_form, "<bold color=0xffa500 size=20>" + tournament_name + " Rankings</>"};
    title_label.text_align(align::center);
    title_label.format(true);

    listbox rankings_list {view_rankings_form};
    rankings_list.enable_single(true, true); //Prevents the user from selecting multiple rows
    rankings_list.sortable(false);
    populateRankingsList(rankings_list, tournament_name);

    view_rankings_form["title_label"] << title_label;
    view_rankings_form["rankings_listbox"] << rankings_list;
    
    view_rankings_form.collocate();
    view_rankings_form.modality();
}

/*
* Adds the team rankings for the specified tournament to the indicated listbox.
* The teams are sorted by ranking and tiebreaker points.
* 
* @param rankings_list the listbox where the rankings will be added
* @param tournament_name the name of the tournament from which rankings will be calculated
*/
void populateRankingsList(listbox& rankings_list, std::string tournament_name) {
    rankings_list.append_header("Ranking");
    rankings_list.append_header("Team Number");
    rankings_list.append_header("Team Name");
    rankings_list.append_header("Ranking Points");
    rankings_list.append_header("Tiebreaker Points");

    const int TEAM_NAME_COLUMN_WIDTH = 350;
    const int OTHER_COLUMN_WIDTH = 130;

    Sql sql;
    listbox::cat_proxy category = rankings_list.at(0);
    std::map<int, std::pair<int, int>> team_points_map = calculateRankingTiebreakerPoints(tournament_name);
    std::vector<std::vector<int>> points_vector =  sortPointsVector(pointsMapToVector(team_points_map));

    for(int i = 1; i < points_vector.size() + 1; i++) { //Start at 1 to use as ranking index
        std::string team_number = std::to_string(points_vector[i - 1][0]);
        std::string team_name = sql.getTeams("WHERE number = " + team_number)[0].name;
        std::string ranking_points = std::to_string(points_vector[i - 1][1]);
        std::string tiebreaker_points = std::to_string(points_vector[i - 1][2]);
        category.append({std::to_string(i), team_number, team_name, ranking_points, tiebreaker_points});
    }

    for(int i = 0; i < rankings_list.column_size(); i++) {
        rankings_list.column_at(i).width(i == 2 ? TEAM_NAME_COLUMN_WIDTH : OTHER_COLUMN_WIDTH);
        rankings_list.column_at(i).text_align(align::center);
    }
}

/*
* Calculates the ranking points and tiebreaker points for the specified tournament.
* Teams are given two ranking points for a win, one for a tie, and two for a loss.
* Teams are given tiebreaker points equal to the score of the losing alliance.
* 
* @param tournament_name the name of the tournament for which points are to be calculated
* @return the map containing team_number -> pair(ranking_points, tiebreaker_points)
*/
std::map<int, std::pair<int, int>> calculateRankingTiebreakerPoints(std::string tournament_name) {
    Sql sql;
    std::string tournament_id = std::to_string(sql.getTournamentId(tournament_name));
    std::vector<Score> scores = sql.getScores("WHERE tournament_id = " + tournament_id);
    std::map<int, std::pair<int, int>> team_points_map = populateTeamsMap(tournament_name);

    const int RP_FOR_WIN = 2;
    const int RP_FOR_TIE = 1;

    for(Score score : scores) {
        std::pair<int, int> alliance_scores = score.getAllianceScores(); //<red, blue>
        //Match table doesn't have match_number column, so use LIMIT instead to get nth value
        std::string query = "WHERE tournament_id = " + tournament_id + " LIMIT " + std::to_string(score.match_number - 1) + ", 1";
        Match match = sql.getMatches(query)[0]; //Query only returns one result

        //Ranking point pair is <red, blue>
        std::pair<int, int> new_ranking_points = getAllianceRankingPoints(alliance_scores);
        int new_tiebreaker_points = std::min(alliance_scores.first, alliance_scores.second);

        team_points_map[match.red1].first += new_ranking_points.first;
        team_points_map[match.red2].first += new_ranking_points.first;
        team_points_map[match.blue1].first += new_ranking_points.second;
        team_points_map[match.blue2].first += new_ranking_points.second;

        team_points_map[match.red1].second += new_tiebreaker_points;
        team_points_map[match.red2].second += new_tiebreaker_points;
        team_points_map[match.blue1].second += new_tiebreaker_points;
        team_points_map[match.blue2].second += new_tiebreaker_points;
    }

    return team_points_map;
}

/*
* Calculates the number of new ranking points that each team should receive.
* A team receives two ranking points for a win, one for a tie, and zero
* for a loss.
* 
* @param alliance_scores the scores for the red and blue alliances for the match
* @return a pair of <red, blue> ranking points that each alliance earned
*/
std::pair<int, int> getAllianceRankingPoints(std::pair<int, int> alliance_scores) {
    int red_ranking_points = 0;
    int blue_ranking_points = 0;

    const int RP_FOR_WIN = 2;
    const int RP_FOR_TIE = 1;

    if(alliance_scores.first > alliance_scores.second) { //Red alliance wins
        red_ranking_points += RP_FOR_WIN;
    } else if(alliance_scores.first < alliance_scores.second) { //Blue alliance wins
        blue_ranking_points += RP_FOR_WIN;
    } else { //Tie
        red_ranking_points += RP_FOR_TIE;
        blue_ranking_points += RP_FOR_TIE;
    }

    return std::pair<int, int> {red_ranking_points, blue_ranking_points};
}

/*
* Creates an empty map of teams with no ranking points or tiebreaker points.
* The map contains team_number -> pair(0, 0)
* 
* @param tournament_name the name of the tournament from which to generate the team list
* @return the map containing the teams and a pair of (0, 0)
*/
std::map<int, std::pair<int, int>> populateTeamsMap(std::string tournament_name) {
    Sql sql;
    std::vector<Team> teams = sql.getTeamsInTournament(tournament_name);
    std::map<int, std::pair<int, int>> teams_map; //Holds team_number -> <QP, TBP>

    for(Team team : teams) {
        teams_map[team.number] = std::pair<int, int>(0, 0);
    }

    return teams_map;
}

/*
* Converts a map of points (team -> pair(ranking_points, tiebreaker_points) to a 2D vector of
* [[team_number, ranking_points, tiebreaker_points], [], ...]
* 
* @param points_map the map of points that is to be converted
* @param the resulting 2D vector of team_number, RP, and TBP
*/
std::vector<std::vector<int>> pointsMapToVector(std::map<int, std::pair<int, int>> points_map) {
    std::vector <std::vector<int>> points_vector;

    for(auto const& points_map_key : points_map) {
        points_vector.push_back({points_map_key.first, points_map_key.second.first, points_map_key.second.second});
    }

    return points_vector;
}

/*
* Sorts a vector of points, where the vector is in the format [[team_number, RP, TBP], [], ...]
* The teams with the most RP are first, with ties determined by TBP
* 
* @param points_vector the vector that will be sorted
* @return the sorted points vector
*/
std::vector<std::vector<int>> sortPointsVector(std::vector<std::vector<int>> points_vector) {
    //https://stackoverflow.com/a/2758095
    std::sort(points_vector.begin(), points_vector.end(), [](std::vector<int> const& first, std::vector<int> const& second) {
        if(first[1] > second[1]) {
            return true;
        } else if(first[1] == second[1]) {
            return first[2] > second[2];
        }

        return false;
    });

    return points_vector;
}