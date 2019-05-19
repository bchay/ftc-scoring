#pragma once

#include <nana/gui.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include "./../sql/sql.h"

using namespace nana;

void showAddTeamsForm(form& container_form, listbox& match_list, std::string);
void populateTeamList(textbox& team_textbox, std::string tournament_name);
void saveTeamList(form& container_form, textbox& teams_textbox, listbox& match_list, std::string tournament_name);
bool showConfirmationPopup(form& container_form, std::string prompt);