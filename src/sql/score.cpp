#include "score.h"

Score::Score(int tournament_id,
             int match_number,
             int red_landed_count,
             int red_parked_count,
             int red_sampled_count,
             int red_minerals_lander_count,
             int red_minerals_depot_count,
             int red_crater_partial_count,
             int red_crater_complete_count,
             int red_latched_count,
             int blue_landed_count,
             int blue_parked_count,
             int blue_sampled_count,
             int blue_minerals_lander_count,
             int blue_minerals_depot_count,
             int blue_crater_partial_count,
             int blue_crater_complete_count,
             int blue_latched_count) {
    this->tournament_id = tournament_id;
    this->match_number = match_number;
    this->red_landed_count = red_landed_count;
    this->red_parked_count = red_parked_count;
    this->red_sampled_count = red_sampled_count;
    this->red_minerals_lander_count = red_minerals_lander_count;
    this->red_minerals_depot_count = red_minerals_depot_count;
    this->red_crater_partial_count = red_crater_partial_count;
    this->red_crater_complete_count = red_crater_complete_count;
    this->red_latched_count = red_latched_count;
    this->blue_landed_count = blue_landed_count;
    this->blue_parked_count = blue_parked_count;
    this->blue_sampled_count = blue_sampled_count;
    this->blue_minerals_lander_count = blue_minerals_lander_count;
    this->blue_minerals_depot_count = blue_minerals_depot_count;
    this->blue_crater_partial_count = blue_crater_partial_count;
    this->blue_crater_complete_count = blue_crater_complete_count;
    this->blue_latched_count = blue_latched_count;
}

std::pair<int, int> Score::getAllianceScores() {
    int red_score = 0;
    red_score += LANDING_SCORE * red_landed_count;
    red_score += PARKING_AUTO_SCORE * red_parked_count;
    red_score += SAMPLING_SCORE * red_sampled_count;
    red_score += LANDER_MINERAL_SCORE * red_minerals_lander_count;
    red_score += DEPOT_MINERAL_SCORE * red_minerals_depot_count;
    red_score += PARTIAL_PARK_SCORE * red_crater_partial_count;
    red_score += COMPLETE_PARK_SCORE * red_crater_complete_count;
    red_score += LATCHED_SCORE * red_latched_count;

    int blue_score = 0;
    blue_score += LANDING_SCORE * blue_landed_count;
    blue_score += PARKING_AUTO_SCORE * blue_parked_count;
    blue_score += SAMPLING_SCORE * blue_sampled_count;
    blue_score += LANDER_MINERAL_SCORE * blue_minerals_lander_count;
    blue_score += DEPOT_MINERAL_SCORE * blue_minerals_depot_count;
    blue_score += PARTIAL_PARK_SCORE * blue_crater_partial_count;
    blue_score += COMPLETE_PARK_SCORE * blue_crater_complete_count;
    blue_score += LATCHED_SCORE * blue_latched_count;

    return std::pair<int, int>(red_score, blue_score);
}