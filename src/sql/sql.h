#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include "tournament.h"
#include "team.h"
#include "match.h"
#include "score.h"

class Sql {
    public:
    Sql();
    Sql(std::string database_address);

    std::vector<Tournament> getTournaments(std::string query);
    void addTournament(std::string name);
    void deleteTournament(std::string tournament_name);
    int getTournamentId(std::string tournament_name);

    std::vector<Team> getTeams(std::string query);
    std::vector<Team> getTeamsInTournament(std::string tournament_name);
    void addTeams(std::vector<Team> teams);
    void deleteTeams();

    std::vector<Match> getMatches(std::string query);
    void addMatches(std::vector<Match> matches);
    void deleteMatchesFromTournament(int tournament_id);

    std::vector<Score> getScores(std::string query);
    void addScore(Score score);
    void deleteScore(Score score);
    void deleteScoresFromTournament(int tournament_id);

    private:
    std::string address = "tcp://127.0.0.1:3306";
    std::string username = "root";
    std::string password = "password";
    std::string database_name = "ftc-scores";

    sql::Driver *driver;
    sql::Connection *connection;
};