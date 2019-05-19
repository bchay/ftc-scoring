#include "test/test.hpp"
#include "src/sql/sql.h"

Sql sql("ftc-scores-test-db");

TEST_CASE("Tournament") {
    SECTION("Add Tournament") {
        sql.addTournament("Test Tournament");
        REQUIRE(sql.getTournaments("").size() == 1);
    }

    SECTION("Get Tournament") {
        REQUIRE(sql.getTournaments("").size() == 1);
        REQUIRE(sql.getTournaments("WHERE name = 'Test Tournament'").size() == 1);
        REQUIRE(sql.getTournaments("WHERE name = 'Does Not Exist'").size() == 0);
    }

    SECTION("Get Tournament Query") {
        REQUIRE(sql.getTournaments("WHERE name = 'Test Tournament'").size() == 1);
    }

    SECTION("Get Nonexistent Tournament") {
        REQUIRE(sql.getTournaments("WHERE name = 'Does Not Exist'").size() == 0);
    }

    SECTION("Get Tournament ID") {
        //ID is auto-incrementing, so there is no expected result
        //The value is auto-incrementing, so it will not be zero
        REQUIRE(sql.getTournamentId("Test Tournament") != 0);
    }

    SECTION("Delete Tournament") {
        sql.deleteTournament("Test Tournament");
        REQUIRE(sql.getTournaments("").size() == 0);
    }
}

TEST_CASE("Team") {
    SECTION("Add Team") {
        std::vector<Team> teams {Team(1, "Team One"), Team(2, "Team Two")};
        sql.addTeams(teams);
        REQUIRE(sql.getTeams("").size() == 2);
    }

    SECTION("Get Team") {
        REQUIRE(sql.getTeams("").size() == 2);
    }

    SECTION("Get Team Query") {
        REQUIRE(sql.getTeams("WHERE name = 'Team One'").size() == 1);
    }

    SECTION("Get Nonexistent Team") {
        REQUIRE(sql.getTeams("WHERE name = 'Does Not Exist'").size() == 0);

        //At end of Team SECTION, delete all created teams
        sql.deleteTeams();
    }
}

TEST_CASE("Match") {
    SECTION("Add Matches") {
        sql.addTournament("Test Tournament");
        int tournament_id = sql.getTournamentId(sql.getTournaments("")[0].name);

        std::vector<Match> matches {Match(tournament_id, 1, 2, 3, 4)};
        sql.addMatches(matches);

        REQUIRE(sql.getMatches("").size() == 1);
    }

    SECTION("Get Matches") {
        std::vector<Match> matches = sql.getMatches("");

        REQUIRE(matches.size() == 1);
        REQUIRE(matches[0].red1 == 1);
    }

    SECTION("Delete Matches From Tournament") {
        int tournament_id = sql.getTournamentId(sql.getTournaments("")[0].name);
        sql.deleteMatchesFromTournament(tournament_id);

        REQUIRE(sql.getMatches("").size() == 0);
    }
}

TEST_CASE("Score") {
    //Run for each SECTION
    int tournament_id = sql.getTournamentId(sql.getTournaments("")[0].name);

    SECTION("Add Score") {
        //Tournament is still in DB from above
        Score score1(tournament_id, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        Score score2(tournament_id, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        Score score3(tournament_id, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        sql.addScore(score1);
        sql.addScore(score2);
        sql.addScore(score3);

        REQUIRE(sql.getScores("").size() == 3);
    }

    SECTION("Get Scores") {
        std::vector<Score> scores = sql.getScores("");
        REQUIRE(scores.size() == 3);
        REQUIRE(scores[0].match_number == 1);
    }

    SECTION("Delete Score") {
        Score score(tournament_id, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        sql.deleteScore(score);
        REQUIRE(sql.getScores("").size() == 2);
    }

    SECTION("Delete Scores From Tournament") {
        sql.deleteMatchesFromTournament(tournament_id);
        REQUIRE(sql.getScores("").size() == 0);

        //Remove tournament at end of tests
        sql.deleteTournament("Test Tournament");
    }
}