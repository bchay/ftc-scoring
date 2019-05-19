#pragma once

#include <nana/gui.hpp>
#include <nana/gui/widgets/listbox.hpp>

using namespace nana;

void showTournamentDetailsForm(form& container_form, std::string tournament_name);
void populateMatchList(listbox& match_list, std::string tournament_name);