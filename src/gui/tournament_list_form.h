#pragma once

#include "./../sql/sql.h"
#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/button.hpp>

using namespace nana;

void showTournamentListForm();
void populateTournaments(listbox& list);
void deleteTournamentClick(std::string tournament_name, listbox& tournament_list);