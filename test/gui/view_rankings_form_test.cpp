#include "test/test.hpp"
#include "src/gui/view_rankings_form.h"

TEST_CASE("Update Ranking Points") {
    std::pair<int, int> alliance_scores = {100, 200};
    std::pair<int, int> ranking_points = getAllianceRankingPoints(alliance_scores);
    REQUIRE(ranking_points.first == 0);
    REQUIRE(ranking_points.second == 2);
}

TEST_CASE("Points Map to Vector") {
    std::map<int, std::pair<int, int>> points_map;
    points_map[1] = std::pair<int, int>(3, 4);
    points_map[2] = std::pair<int, int>(5, 6);

    std::vector<std::vector<int>> points_vector = pointsMapToVector(points_map);
    //points_vector is formatted as [[team_number, RP, TBP], [], ...]
    REQUIRE(points_vector[0][0] == 1);
    REQUIRE(points_vector[0][1] == 3);
    REQUIRE(points_vector[0][2] == 4);
    REQUIRE(points_vector[1][0] == 2);
    REQUIRE(points_vector[1][1] == 5);
    REQUIRE(points_vector[1][2] == 6);
}

TEST_CASE("Sort Points Vector") {
    std::vector<std::vector<int>> points_vector = {{0, 0, 0}, {1, 2, 0}, {2, 2, 1}, {3, 4, 0}};
    std::vector<std::vector<int>> expected_sorted_points_vector = {{3, 4, 0}, {2, 2, 1}, {1, 2, 0}, {0, 0, 0}};
    std::vector<std::vector<int>> actual_sorted_points_vector = sortPointsVector(points_vector);

    REQUIRE(expected_sorted_points_vector == actual_sorted_points_vector);
}