#pragma once
#include "./../sql/sql.h"
#include <nana/gui.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <string>

using namespace nana;

void showAddTournamentForm(form& container_form, listbox& tournament_list);
void saveButtonClick(textbox& name_textbox, listbox& tournament_list);