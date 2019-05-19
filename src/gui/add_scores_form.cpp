#include "add_scores_form.h"
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/button.hpp>
#include "./../sql/sql.h"

/*
* Shows the form that allows users to enter scoring information
* for a particular match. The scoring is based around the FTC
* Rover Ruckus 2018 - 2019 season. Scores can be added for both the red
* and blue alliances. Any previously saved scores are automatically populated.
* 
* @param container_form the form from which this method is called
*                  this form is the match list form
* @param tournament_name the name of the tournament to which the scores belong
* @param match_number the match number to which the score belongs
*/
void showAddScoresForm(form& container_form, std::string tournament_name, std::string match_number) {
    /*
    The Nana objects have their copy constructors deleted, and so cannot be returned as functions.
    Likewise, a widget cannot be constructed in one function and added to a form passed as a parameter
    because the widget will go out of scope when the function call ends and will not appear. Thus,
    all widgets must be created in the same function where the form itself is created. However, once
    an widget is created, it can have data added to it from another function.

    Labels must be unique even if the contents are identical. Thus, both the red and blue alliances
    must have individual label objects.

    Owing to these concerns, this function has a high degree of repetition. This is unavoidable due to
    the nature of Nana.
    */

    form add_scores_form(rectangle {screen().desktop_size()});
    API::fullscreen(container_form, true);

    const char* form_layout =
        "vert"
        "<weight = 10% title_label>"
        "<weight = 70%"
            "<weight = 15%>"
            "<vert weight = 30%"
                "<weight = 10% red_alliance_label>"
                "<weight = 5%>"
                "<weight = 5%"
                    "<>"
                    "<weight = 50% red_landed_label>"
                    "<weight = 30% red_landed_spinbox>"
                    "<>"
                ">"
                "<>"
                "<weight = 5%"
                    "<>"
                    "<weight = 50% red_parked_label>"
                    "<weight = 30% red_parked_spinbox>"
                    "<>"
                ">"
                "<>"
                "<weight = 5%"
                    "<>"
                    "<weight = 50% red_sampled_label>"
                    "<weight = 30% red_sampled_spinbox>"
                    "<>"
                ">"
                "<>"
                "<weight = 5%"
                    "<>"
                    "<weight = 50% red_minerals_lander_label>"
                    "<weight = 30% red_minerals_lander_spinbox>"
                    "<>"
                ">"
                "<>"
                "<weight = 5%"
                    "<>"
                    "<weight = 50% red_minerals_depot_label>"
                    "<weight = 30% red_minerals_depot_spinbox>"
                    "<>"
                ">"
                "<>"
                "<weight = 5%"
                    "<>"
                    "<weight = 50% red_crater_partial_label>"
                    "<weight = 30% red_crater_partial_spinbox>"
                    "<>"
                ">"
                "<>"
                "<weight = 5%"
                    "<>"
                    "<weight = 50% red_crater_complete_label>"
                    "<weight = 30% red_crater_complete_spinbox>"
                    "<>"
                ">"
                "<>"
                "<weight = 5%"
                    "<>"
                    "<weight = 50% red_latched_label>"
                    "<weight = 30% red_latched_spinbox>"
                    "<>"
                ">"
            ">"
            "<>"
            "<vert weight = 30%"
                "<weight = 10% blue_alliance_label>"
                "<weight = 5%>"
                "<weight = 5%"
                    "<>"
                    "<weight = 50% blue_landed_label>"
                    "<weight = 30% blue_landed_spinbox>"
                    "<>"
                ">"
                "<>"
                "<weight = 5%"
                    "<>"
                    "<weight = 50% blue_parked_label>"
                    "<weight = 30% blue_parked_spinbox>"
                    "<>"
                ">"
                "<>"
                "<weight = 5%"
                    "<>"
                    "<weight = 50% blue_sampled_label>"
                    "<weight = 30% blue_sampled_spinbox>"
                    "<>"
                ">"
                "<>"
                "<weight = 5%"
                    "<>"
                    "<weight = 50% blue_minerals_lander_label>"
                    "<weight = 30% blue_minerals_lander_spinbox>"
                    "<>"
                ">"
                "<>"
                "<weight = 5%"
                    "<>"
                    "<weight = 50% blue_minerals_depot_label>"
                    "<weight = 30% blue_minerals_depot_spinbox>"
                    "<>"
                ">"
                "<>"
                "<weight = 5%"
                    "<>"
                    "<weight = 50% blue_crater_partial_label>"
                    "<weight = 30% blue_crater_partial_spinbox>"
                    "<>"
                ">"
                "<>"
                "<weight = 5%"
                    "<>"
                    "<weight = 50% blue_crater_complete_label>"
                    "<weight = 30% blue_crater_complete_spinbox>"
                    "<>"
                ">"
                "<>"
                "<weight = 5%"
                    "<>"
                    "<weight = 50% blue_latched_label>"
                    "<weight = 30% blue_latched_spinbox>"
                    "<>"
                ">"
            ">"
            "<weight = 15%>"
        ">"
        "<weight = 5%>"
        "<weight = 5%"
            "<>"
            "<weight = 10% save_scores_button>"
            "<>"
        ">"
        "<weight = 5%>";

    add_scores_form.div(form_layout);
    add_scores_form.caption("Match " + match_number);
    add_scores_form.bgcolor(colors::white);

    label title_label {add_scores_form};
    title_label.caption("<bold color=0xffa500 size = 20>Match " + match_number + "</>");
    title_label.text_align(nana::align::center);
    title_label.format(true);

    //Red Alliance Labels and Spinboxes
    label red_alliance_label {add_scores_form};
    red_alliance_label.caption("<bold red size = 16>Red Alliance</>");
    red_alliance_label.text_align(align::center);
    red_alliance_label.format(true);

    label red_landed_label {add_scores_form, "<size = 14 center>Robots Landed:</>"};
    red_landed_label.format(true);
    label red_parked_label {add_scores_form, "<size = 14 center>Robots parked:</>"};
    red_parked_label.format(true);
    label red_sampled_label {add_scores_form, "<size = 14 center>Samples completed:</>"};
    red_sampled_label.format(true);
    label red_minerals_lander_label {add_scores_form, "<size = 14 center>Minerals in lander:</>"};
    red_minerals_lander_label.format(true);
    label red_minerals_depot_label {add_scores_form, "<size = 14 center>Minerals in depot:</>"};
    red_minerals_depot_label.format(true);
    label red_crater_partial_label {add_scores_form, "<size = 14 center>Robots partially in crater:</>"};
    red_crater_partial_label.format(true);
    label red_crater_complete_label {add_scores_form, "<size = 14 center>Robots completely in crater:</>"};
    red_crater_complete_label.format(true);
    label red_latched_label {add_scores_form, "<size = 14 center>Robots latched:</>"};
    red_latched_label.format(true);

    spinbox red_landed_spinbox {add_scores_form};
    red_landed_spinbox.range(0, NUM_ROBOTS_PER_ALLIANCE, 1);
    spinbox red_parked_spinbox {add_scores_form};
    red_parked_spinbox.range(0, NUM_ROBOTS_PER_ALLIANCE, 1);
    spinbox red_sampled_spinbox {add_scores_form};
    red_sampled_spinbox.range(0, NUM_ROBOTS_PER_ALLIANCE, 1);
    spinbox red_minerals_lander_spinbox {add_scores_form};
    red_minerals_lander_spinbox.range(0, MAX_MINERALS, 1);
    spinbox red_minerals_depot_spinbox {add_scores_form};
    red_minerals_depot_spinbox.range(0, MAX_MINERALS, 1);
    spinbox red_crater_partial_spinbox {add_scores_form};
    red_crater_partial_spinbox.range(0, NUM_ROBOTS_PER_ALLIANCE, 1);
    spinbox red_crater_complete_spinbox {add_scores_form};
    red_crater_complete_spinbox.range(0, NUM_ROBOTS_PER_ALLIANCE, 1);
    spinbox red_latched_spinbox {add_scores_form};
    red_latched_spinbox.range(0, NUM_ROBOTS_PER_ALLIANCE, 1);

    //Blue Alliance Labels and Spinboxes
    label blue_alliance_label {add_scores_form};
    blue_alliance_label.caption("<bold blue size = 16>Blue Alliance</>");
    blue_alliance_label.text_align(align::center);
    blue_alliance_label.format(true);

    label blue_landed_label {add_scores_form, "<size = 14 center>Robots Landed:</>"};
    blue_landed_label.format(true);
    label blue_parked_label {add_scores_form, "<size = 14 center>Robots parked:</>"};
    blue_parked_label.format(true);
    label blue_sampled_label {add_scores_form, "<size = 14 center>Samples completed:</>"};
    blue_sampled_label.format(true);
    label blue_minerals_lander_label {add_scores_form, "<size = 14 center>Minerals in lander:</>"};
    blue_minerals_lander_label.format(true);
    label blue_minerals_depot_label {add_scores_form, "<size = 14 center>Minerals in depot:</>"};
    blue_minerals_depot_label.format(true);
    label blue_crater_partial_label {add_scores_form, "<size = 14 center>Robots partially in crater:</>"};
    blue_crater_partial_label.format(true);
    label blue_crater_complete_label {add_scores_form, "<size = 14 center>Robots completely in crater:</>"};
    blue_crater_complete_label.format(true);
    label blue_latched_label {add_scores_form, "<size = 14 center>Robots latched:</>"};
    blue_latched_label.format(true);

    spinbox blue_landed_spinbox {add_scores_form};
    blue_landed_spinbox.range(0, NUM_ROBOTS_PER_ALLIANCE, 1);
    spinbox blue_parked_spinbox {add_scores_form};
    blue_parked_spinbox.range(0, NUM_ROBOTS_PER_ALLIANCE, 1);
    spinbox blue_sampled_spinbox {add_scores_form};
    blue_sampled_spinbox.range(0, NUM_ROBOTS_PER_ALLIANCE, 1);
    spinbox blue_minerals_lander_spinbox {add_scores_form};
    blue_minerals_lander_spinbox.range(0, MAX_MINERALS, 1);
    spinbox blue_minerals_depot_spinbox {add_scores_form};
    blue_minerals_depot_spinbox.range(0, MAX_MINERALS, 1);
    spinbox blue_crater_partial_spinbox {add_scores_form};
    blue_crater_partial_spinbox.range(0, NUM_ROBOTS_PER_ALLIANCE, 1);
    spinbox blue_crater_complete_spinbox {add_scores_form};
    blue_crater_complete_spinbox.range(0, NUM_ROBOTS_PER_ALLIANCE, 1);
    spinbox blue_latched_spinbox {add_scores_form};
    blue_latched_spinbox.range(0, NUM_ROBOTS_PER_ALLIANCE, 1);

    button save_scores_button {add_scores_form};
    save_scores_button.caption("Save Scores");
    save_scores_button.events().click([&] {
        int red_landed_count = red_landed_spinbox.to_int();
        int red_parked_count = red_parked_spinbox.to_int();
        int red_sampled_count = red_sampled_spinbox.to_int();
        int red_minerals_lander_count = red_minerals_lander_spinbox.to_int();
        int red_minerals_depot_count = red_minerals_depot_spinbox.to_int();
        int red_crater_partial_count = red_crater_partial_spinbox.to_int();
        int red_crater_complete_count = red_crater_complete_spinbox.to_int();
        int red_latched_count = red_latched_spinbox.to_int();

        int blue_landed_count = blue_landed_spinbox.to_int();
        int blue_parked_count = blue_parked_spinbox.to_int();
        int blue_sampled_count = blue_sampled_spinbox.to_int();
        int blue_minerals_lander_count = blue_minerals_lander_spinbox.to_int();
        int blue_minerals_depot_count = blue_minerals_depot_spinbox.to_int();
        int blue_crater_partial_count = blue_crater_partial_spinbox.to_int();
        int blue_crater_complete_count = blue_crater_complete_spinbox.to_int();
        int blue_latched_count = blue_latched_spinbox.to_int();

        Sql sql;
        Score score (sql.getTournamentId(tournament_name),
                    std::stoi(match_number),
                    red_landed_count,
                    red_parked_count,
                    red_sampled_count,
                    red_minerals_lander_count,
                    red_minerals_depot_count,
                    red_crater_partial_count,
                    red_crater_complete_count,
                    red_latched_count,
                    blue_landed_count,
                    blue_parked_count,
                    blue_sampled_count,
                    blue_minerals_lander_count,
                    blue_minerals_depot_count,
                    blue_crater_partial_count,
                    blue_crater_complete_count,
                    blue_latched_count);
        std::cout << red_landed_count << " " << score.red_landed_count;
        sql.addScore(score);
        add_scores_form.close();
    });

    setDefaultValues(tournament_name,
                          match_number,
                          red_landed_spinbox,
                          red_parked_spinbox,
                          red_sampled_spinbox,
                          red_minerals_lander_spinbox,
                          red_minerals_depot_spinbox,
                          red_crater_partial_spinbox,
                          red_crater_complete_spinbox,
                          red_latched_spinbox,
                          blue_landed_spinbox,
                          blue_parked_spinbox,
                          blue_sampled_spinbox,
                          blue_minerals_lander_spinbox,
                          blue_minerals_depot_spinbox,
                          blue_crater_partial_spinbox,
                          blue_crater_complete_spinbox,
                          blue_latched_spinbox);

    add_scores_form["title_label"] << title_label;

    add_scores_form["red_alliance_label"] << red_alliance_label;
    add_scores_form["red_landed_label"] << red_landed_label;
    add_scores_form["red_parked_label"] << red_parked_label;
    add_scores_form["red_sampled_label"] << red_sampled_label;
    add_scores_form["red_minerals_lander_label"] << red_minerals_lander_label;
    add_scores_form["red_minerals_depot_label"] << red_minerals_depot_label;
    add_scores_form["red_crater_partial_label"] << red_crater_partial_label;
    add_scores_form["red_crater_complete_label"] << red_crater_complete_label;
    add_scores_form["red_latched_label"] << red_latched_label;

    add_scores_form["red_landed_spinbox"] << red_landed_spinbox;
    add_scores_form["red_parked_spinbox"] << red_parked_spinbox;
    add_scores_form["red_sampled_spinbox"] << red_sampled_spinbox;
    add_scores_form["red_minerals_lander_spinbox"] << red_minerals_lander_spinbox;
    add_scores_form["red_minerals_depot_spinbox"] << red_minerals_depot_spinbox;
    add_scores_form["red_crater_partial_spinbox"] << red_crater_partial_spinbox;
    add_scores_form["red_crater_complete_spinbox"] << red_crater_complete_spinbox;
    add_scores_form["red_latched_spinbox"] << red_latched_spinbox;

    add_scores_form["blue_landed_label"] << blue_landed_label;
    add_scores_form["blue_parked_label"] << blue_parked_label;
    add_scores_form["blue_sampled_label"] << blue_sampled_label;
    add_scores_form["blue_minerals_lander_label"] << blue_minerals_lander_label;
    add_scores_form["blue_minerals_depot_label"] << blue_minerals_depot_label;
    add_scores_form["blue_crater_partial_label"] << blue_crater_partial_label;
    add_scores_form["blue_crater_complete_label"] << blue_crater_complete_label;
    add_scores_form["blue_latched_label"] << blue_latched_label;

    add_scores_form["blue_landed_spinbox"] << blue_landed_spinbox;
    add_scores_form["blue_parked_spinbox"] << blue_parked_spinbox;
    add_scores_form["blue_sampled_spinbox"] << blue_sampled_spinbox;
    add_scores_form["blue_minerals_lander_spinbox"] << blue_minerals_lander_spinbox;
    add_scores_form["blue_minerals_depot_spinbox"] << blue_minerals_depot_spinbox;
    add_scores_form["blue_crater_partial_spinbox"] << blue_crater_partial_spinbox;
    add_scores_form["blue_crater_complete_spinbox"] << blue_crater_complete_spinbox;
    add_scores_form["blue_latched_spinbox"] << blue_latched_spinbox;

    add_scores_form["save_scores_button"] << save_scores_button;
    add_scores_form.collocate();
    add_scores_form.modality();
}

void setDefaultValues(std::string tournament_name, 
                      std::string match_number,
                      spinbox& red_landed_spinbox,
                      spinbox& red_parked_spinbox,
                      spinbox& red_sampled_spinbox,
                      spinbox& red_minerals_lander_spinbox,
                      spinbox& red_minerals_depot_spinbox,
                      spinbox& red_crater_partial_spinbox,
                      spinbox& red_crater_complete_spinbox,
                      spinbox& red_latched_spinbox,
                      spinbox& blue_landed_spinbox,
                      spinbox& blue_parked_spinbox,
                      spinbox& blue_sampled_spinbox,
                      spinbox& blue_minerals_lander_spinbox,
                      spinbox& blue_minerals_depot_spinbox,
                      spinbox& blue_crater_partial_spinbox,
                      spinbox& blue_crater_complete_spinbox,
                      spinbox& blue_latched_spinbox) {
    Sql sql;
    std::string tournament_id = std::to_string(sql.getTournamentId(tournament_name));
    std::string query = "WHERE tournament_id = '" + tournament_id + "' AND match_number = '" + match_number + "'";
    std::vector<Score> scores = sql.getScores(query);

    //If there are already scores saved for this match, fill in the Spinners with
    //the old values
    if(scores.size() == 1) {
        Score score = scores[0];
        red_landed_spinbox.value(std::to_string(score.red_landed_count));
        red_parked_spinbox.value(std::to_string(score.red_parked_count));
        red_sampled_spinbox.value(std::to_string(score.red_sampled_count));
        red_minerals_lander_spinbox.value(std::to_string(score.red_minerals_lander_count));
        red_minerals_depot_spinbox.value(std::to_string(score.red_minerals_depot_count));
        red_crater_partial_spinbox.value(std::to_string(score.red_crater_partial_count));
        red_crater_complete_spinbox.value(std::to_string(score.red_crater_complete_count));
        red_crater_complete_spinbox.value(std::to_string(score.red_latched_count));

        blue_landed_spinbox.value(std::to_string(score.blue_landed_count));
        blue_parked_spinbox.value(std::to_string(score.blue_parked_count));
        blue_sampled_spinbox.value(std::to_string(score.blue_sampled_count));
        blue_minerals_lander_spinbox.value(std::to_string(score.blue_minerals_lander_count));
        blue_minerals_depot_spinbox.value(std::to_string(score.blue_minerals_depot_count));
        blue_crater_partial_spinbox.value(std::to_string(score.blue_crater_partial_count));
        blue_crater_complete_spinbox.value(std::to_string(score.blue_crater_complete_count));
        blue_crater_complete_spinbox.value(std::to_string(score.blue_latched_count));
    }
}