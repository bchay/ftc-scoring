#pragma once

#include <nana/gui.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <vector>
#include <map>
#include <string>

using namespace nana;

void showRankingsForm(form& container_form, std::string tournament_name);
void populateRankingsList(listbox& rankings_list, std::string tournament_name);
std::pair<int, int> getAllianceRankingPoints(std::pair<int, int> alliance_scores);
std::map<int, std::pair<int, int>> calculateRankingTiebreakerPoints(std::string tournament_name);
std::map<int, std::pair<int, int>> populateTeamsMap(std::string tournament_name);
std::vector<std::vector<int>> pointsMapToVector(std::map<int, std::pair<int, int>> points_map);
std::vector<std::vector<int>> sortPointsVector(std::vector<std::vector<int>> points_vector);