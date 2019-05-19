#pragma once

#include <utility>

class Score {
    public:
    const int LANDING_SCORE = 30;
    const int SAMPLING_SCORE = 25;
    const int CLAIMING_SCORE = 15;
    const int PARKING_AUTO_SCORE = 10;
    const int LANDER_MINERAL_SCORE = 5;
    const int DEPOT_MINERAL_SCORE = 2;
    const int LATCHED_SCORE = 50;
    const int PARTIAL_PARK_SCORE = 15;
    const int COMPLETE_PARK_SCORE = 25;

    int tournament_id;
    int match_number;
    int red_landed_count;
    int red_parked_count;
    int red_sampled_count;
    int red_minerals_lander_count;
    int red_minerals_depot_count;
    int red_crater_partial_count;
    int red_crater_complete_count;
    int red_latched_count;
    int blue_landed_count;
    int blue_parked_count;
    int blue_sampled_count;
    int blue_minerals_lander_count;
    int blue_minerals_depot_count;
    int blue_crater_partial_count;
    int blue_crater_complete_count;
    int blue_latched_count;
    
    Score(int tournament_id,
          int match_number,
          int red_landed_count,
          int red_parked_count,
          int red_sampled_count,
          int red_minerals_lander_count,
          int red_minerals_depot_count,
          int red_crater_partial_count,
          int red_crater_in_count,
          int red_latched_count,
          int blue_landed_count,
          int blue_parked_count,
          int blue_sampled_count,
          int blue_minerals_lander_count,
          int blue_minerals_depot_count,
          int blue_crater_partial_count,
          int blue_crater_in_count,
          int blue_latched_count);
    
    std::pair<int, int> getAllianceScores();
};