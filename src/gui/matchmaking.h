#pragma once

#include <vector>
#include "./../sql/sql.h"
#include <nana/gui.hpp>
#include <nana/gui/widgets/listbox.hpp>

using namespace nana;

const double NUM_MATCHES_PER_TEAM = 5; //double to avoid truncation when dividing
const double NUM_TEAMS_PER_MATCH = 4;

void generateMatchSchedule(form& container_form, listbox& match_list, std::vector<Team> teams, std::string tournament_name);
bool assignTeamsToAlliance(std::vector<Team> teams, std::vector<std::pair<int, int>>& match_indices, std::vector<Match>& matches);
bool assignTeamToAlliance(Team team, std::vector<std::pair<int, int>>& match_indices, std::vector<Match>& matches);
std::vector<std::pair<int, int>> populateMatchIndices(int matches_in_tournament);
void retryMatchGeneration(form& container_form, listbox& match_list, std::vector<Team> teams, std::string tournament_name);
void addSurrogates(std::vector<Team> teams, std::vector<Match>& match_list, std::vector<std::pair<int, int>> match_indices);
std::vector<Match> setAllianceTeam(std::vector<Match> matches, int match_number, int team_number, int alliance_index, bool surrogate);