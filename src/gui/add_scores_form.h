#pragma once

#include <nana/gui.hpp>
#include <nana/gui/widgets/spinbox.hpp>

const int NUM_ROBOTS_PER_ALLIANCE = 2;
const int MAX_MINERALS = 150;

using namespace nana;

void showAddScoresForm(form& container_form, std::string tournament_name, std::string match_number);
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
                      spinbox& blue_latched_spinbox);