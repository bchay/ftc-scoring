#include "sql.h"

/*
The ftc-scores database can be created with the following schema:
CREATE TABLE `teams` (
   `number` int(11) NOT NULL,
   `name` varchar(64) NOT NULL,
   PRIMARY KEY (`number`),
   UNIQUE KEY `number_UNIQUE` (`number`)
 ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci

 CREATE TABLE `tournaments` (
   `id` int(11) NOT NULL AUTO_INCREMENT,
   `name` varchar(256) NOT NULL,
   PRIMARY KEY (`id`),
   UNIQUE KEY `name_UNIQUE` (`name`)
 ) ENGINE=InnoDB AUTO_INCREMENT=13273 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci

 CREATE TABLE `scores` (
   `id` int(11) NOT NULL AUTO_INCREMENT,
   `tournament_id` int(11) NOT NULL,
   `match_number` int(11) NOT NULL,
   `red_landed_count` int(11) NOT NULL,
   `red_parked_count` int(11) NOT NULL,
   `red_sampled_count` int(11) NOT NULL,
   `red_minerals_lander_count` int(11) NOT NULL,
   `red_minerals_depot_count` int(11) NOT NULL,
   `red_crater_partial_count` int(11) NOT NULL,
   `red_crater_complete_count` int(11) NOT NULL,
   `red_latched_count` int(11) NOT NULL,
   `blue_landed_count` int(11) NOT NULL,
   `blue_parked_count` int(11) NOT NULL,
   `blue_sampled_count` int(11) NOT NULL,
   `blue_minerals_lander_count` int(11) NOT NULL,
   `blue_minerals_depot_count` int(11) NOT NULL,
   `blue_crater_partial_count` int(11) NOT NULL,
   `blue_crater_complete_count` int(11) NOT NULL,
   `blue_latched_count` int(11) NOT NULL,
   PRIMARY KEY (`id`)
 ) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci

 CREATE TABLE `matches` (
   `id` int(11) NOT NULL AUTO_INCREMENT,
   `tournament_id` int(11) NOT NULL,
   `red1` int(11) NOT NULL,
   `red2` int(11) NOT NULL,
   `blue1` int(11) NOT NULL,
   `blue2` int(11) NOT NULL,
   `red1_surrogate` tinyint(4) NOT NULL,
   `red2_surrogate` tinyint(4) NOT NULL,
   `blue1_surrogate` tinyint(4) NOT NULL,
   `blue2_surrogate` tinyint(4) NOT NULL,
   PRIMARY KEY (`id`)
 ) ENGINE=InnoDB AUTO_INCREMENT=954 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci
*/

//https://dev.mysql.com/doc/connector-cpp/1.1/en/connector-cpp-examples-complete-example-1.html
Sql::Sql() {
        driver = get_driver_instance();
        connection = driver->connect(address, username, password);
        connection->setSchema(database_name);
}

Sql::Sql(std::string database_name) {
    driver = get_driver_instance();
    connection = driver->connect(address, username, password);
    connection->setSchema(database_name);
}

/*
* Returns a list of Tournament objects that represent the rows of data from the tournaments SQL table.
*
* @param query Any aspect of the SQL query that goes after FROM tournaments, ie. WHERE, ORDER BY, etc.
* @return vector of Tournament objects containing data for each row
*/
std::vector<Tournament> Sql::getTournaments(std::string query) {
    std::vector<Tournament> tournaments;

    sql::Statement *statement = connection->createStatement();
    sql::ResultSet *result = statement->executeQuery("SELECT name FROM tournaments " + query);
    
    while(result->next()) {
        std::string name = result->getString("name");

        Tournament tournament(name);
        tournaments.push_back(tournament);
    }

    delete statement;
    delete result;

    return tournaments;
}

/*
* Adds a tournament to the tournaments database.
* This method uses prepared statements to protect against SQL injection attacks.
* The tournament name must be unique.
*
* @param the name of the tournament to be added
*/
void Sql::addTournament(std::string name) {
    std::string query = "INSERT INTO tournaments(name) VALUES(?)";
    sql::PreparedStatement *prepared_statement = connection->prepareStatement(query);
    prepared_statement->setString(1, name);
    prepared_statement->executeUpdate();

    delete prepared_statement;
}

/*
* Deletes a tournament along with all associated scores and matches.
* 
* @param tournament_name the name of the tournament that will be deleted
*/
void Sql::deleteTournament(std::string tournament_name) {
    sql::Statement *statement = connection->createStatement();
    statement->executeUpdate("DELETE FROM tournaments WHERE name = '" + tournament_name + "'");

    delete statement;
}

/*
* Retrieves the id of a tournament from its name
*
* @param tournament_name the name of the tournament whose id is to be retrieved
* @return the id of the indicated tournament
*/
int Sql::getTournamentId(std::string tournament_name) {
    sql::Statement *statement = connection->createStatement();
    sql::ResultSet *result = statement->executeQuery("SELECT id FROM tournaments WHERE name = '" + tournament_name + "'");

    result->next();
    int tournament_id = result->getInt("id");

    delete statement;
    delete result;

    return tournament_id;
}

/*
* Returns a list of Team objects that represent the rows of data from the teams SQL table.
*
* @param query Any aspect of the SQL query that goes after FROM teams, ie. WHERE, ORDER BY, etc.
* @return vector of Team objects containing data for each row
*/
std::vector<Team> Sql::getTeams(std::string query) {
    std::vector<Team> teams;

    sql::Statement *statement = connection->createStatement();
    sql::ResultSet *result = statement->executeQuery("SELECT number, name FROM teams " + query);

    while(result->next()) {
        int number = result->getInt("number");
        std::string name = result->getString("name");

        Team team(number, name);
        teams.push_back(team);
    }

    delete statement;
    delete result;

    return teams;
}

/*
* Returns a list of all teams that are competing in the specified tournament.
* 
* tournament_name the name of the tournament in which teams are competing
*/
std::vector<Team> Sql::getTeamsInTournament(std::string tournament_name) {
    std::vector<Team> teams;
    std::string tournament_id = std::to_string(getTournamentId(tournament_name));
    sql::Statement *statement = connection->createStatement();
    std::string query = "SELECT number, name FROM teams WHERE number IN ("
        "SELECT red1 FROM matches WHERE tournament_id = " + tournament_id + " UNION "
        "SELECT red2 FROM matches WHERE tournament_id = " + tournament_id + " UNION "
        "SELECT blue1 FROM matches WHERE tournament_id = " + tournament_id + " UNION "
        "SELECT blue2 FROM matches WHERE tournament_id = " + tournament_id +
        ") ORDER BY number";
    sql::ResultSet *result = statement->executeQuery(query);

    while(result->next()) {
        int number = result->getInt("number");
        std::string name = result->getString("name");

        Team team(number, name);
        teams.push_back(team);
    }

    delete statement;
    delete result;

    return teams;
}

/*
* Adds a list of Team objects to the database.
* This method uses prepared statements to protect against SQL injection.
* The team number is a unique index, if a team is added whose number already
* exists, the INSERT query is ignored.
* 
* @param teams a vector of teams that are to be added to the database
*/
void Sql::addTeams(std::vector<Team> teams) {
    for(Team team : teams) {
        std::string query = "INSERT INTO teams(number, name) VALUES (?, ?)";
        sql::PreparedStatement *prepared_statement = connection->prepareStatement(query);
        prepared_statement->setInt(1, team.number);
        prepared_statement->setString(2, team.name);

        try {
            prepared_statement->executeUpdate();
        } catch(sql::SQLException) {
            //Intentionally left blank
            //If inserting when number (unique key) already exists, an exception is thrown and can be ignored
        }

        delete prepared_statement;
    }
}

/*
* Deletes all teams. This method does not delete the teams database table.
* This method is only called in sql_text.cpp and is only used for testing.
*/
void Sql::deleteTeams() {
    sql::Statement *statement = connection->createStatement();
    statement->executeUpdate("DELETE FROM teams");

    delete statement;
}

/*
* Returns a list of Match objects that represent the rows of data from the matches SQL table.
*
* @param query Any aspect of the SQL query that goes after FROM matches, ie. WHERE, ORDER BY, etc.
* @return vector of Match objects containing data for each row
*/
std::vector<Match> Sql::getMatches(std::string query) {
    std::vector<Match> matches;

    sql::Statement *statement = connection->createStatement();
    sql::ResultSet *result = statement->executeQuery("SELECT * FROM matches " + query);

    while(result->next()) {
        int tournament_id = result->getInt("tournament_id");
        int red1 = result->getInt("red1");
        int red2 = result->getInt("red2");
        int blue1 = result->getInt("blue1");
        int blue2 = result->getInt("blue2");
        bool red1_surrogate = result->getBoolean("red1_surrogate");
        bool red2_surrogate = result->getBoolean("red2_surrogate");
        bool blue1_surrogate = result->getBoolean("blue1_surrogate");
        bool blue2_surrogate = result->getBoolean("blue2_surrogate");

        Match match(tournament_id, red1, red2, blue1, blue2, red1_surrogate, red2_surrogate, blue1_surrogate, blue2_surrogate);
        matches.push_back(match);
    }

    delete statement;
    delete result;

    return matches;
}

/*
* Adds a list of Match objects to the database.
* This method uses prepared statements to protect against SQL injection.
* The matches are added to the tournament as indicated by match.tournament_id
*
* @param matches a vector of matches that are to be added to the database
*/
void Sql::addMatches(std::vector<Match> matches) {
    for(Match match : matches) {
        std::string query = "INSERT INTO matches(tournament_id, red1, red2, blue1, blue2, red1_surrogate, red2_surrogate, blue1_surrogate, blue2_surrogate) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";
        sql::PreparedStatement *prepared_statement = connection->prepareStatement(query);
        prepared_statement->setInt(1, match.tournament_id);
        prepared_statement->setInt(2, match.red1);
        prepared_statement->setInt(3, match.red2);
        prepared_statement->setInt(4, match.blue1);
        prepared_statement->setInt(5, match.blue2);
        prepared_statement->setBoolean(6, match.red1_surrogate);
        prepared_statement->setBoolean(7, match.red2_surrogate);
        prepared_statement->setBoolean(8, match.blue1_surrogate);
        prepared_statement->setBoolean(9, match.blue2_surrogate);
        prepared_statement->executeUpdate();

        delete prepared_statement;
    }
}

/*
* Deletes all matches associated with a particular tournament. This method does not
* delete the tournament itself.
* 
* @param tournament_id the numerical id associated with the tournament whose matches
                       are to be deleted
*/
void Sql::deleteMatchesFromTournament(int tournament_id) {
    sql::Statement *statement = connection->createStatement();
    statement->executeUpdate("DELETE FROM matches WHERE tournament_id = " + std::to_string(tournament_id));

    delete statement;
}

/*
* Returns a list of Score objects that represent the rows of data from the scores SQL table.
*
* @param query Any aspect of the SQL query that goes after FROM scores, ie. WHERE, ORDER BY, etc.
* @return vector of Score objects containing data for each row
*/
std::vector<Score> Sql::getScores(std::string query) {
    std::vector<Score> scores;

    sql::Statement *statement = connection->createStatement();
    sql::ResultSet *result = statement->executeQuery("SELECT * FROM scores " + query);

    while(result->next()) {
        int tournament_id = result->getInt("tournament_id");
        int match_number = result->getInt("match_number");
        int red_landed_count = result->getInt("red_landed_count");
        int red_parked_count = result->getInt("red_parked_count");
        int red_sampled_count = result->getInt("red_sampled_count");
        int red_minerals_lander_count = result->getInt("red_minerals_lander_count");
        int red_minerals_depot_count = result->getInt("red_minerals_depot_count");
        int red_crater_partial_count = result->getInt("red_crater_partial_count");
        int red_crater_in_count = result->getInt("red_crater_complete_count");
        int red_latched_count = result->getInt("red_latched_count");
        int blue_landed_count = result->getInt("blue_landed_count");
        int blue_parked_count = result->getInt("blue_parked_count");
        int blue_sampled_count = result->getInt("blue_sampled_count");
        int blue_minerals_lander_count = result->getInt("blue_minerals_lander_count");
        int blue_minerals_depot_count = result->getInt("blue_minerals_depot_count");
        int blue_crater_partial_count = result->getInt("blue_crater_partial_count");
        int blue_crater_in_count = result->getInt("blue_crater_complete_count");
        int blue_latched_count = result->getInt("blue_latched_count");

        Score score(tournament_id,
                    match_number,
                    red_landed_count,
                    red_parked_count,
                    red_sampled_count,
                    red_minerals_lander_count,
                    red_minerals_depot_count,
                    red_crater_partial_count,
                    red_crater_in_count,
                    red_latched_count,
                    blue_landed_count,
                    blue_parked_count,
                    blue_sampled_count,
                    blue_minerals_lander_count,
                    blue_minerals_depot_count,
                    blue_crater_partial_count,
                    blue_crater_in_count,
                    blue_latched_count); 
        scores.push_back(score);
    }

    delete statement;
    delete result;

    return scores;
}

/*
* Adds a Score object to the database.
* This method uses prepared statements to protect against SQL injection.
* If a score with this tournament_id and match_number already exists, that score will be
* overriden.
*
* @param score the Score that are to be added to the database
*/
void Sql::addScore(Score score) {
    std::string query = "INSERT INTO scores ("
        "tournament_id, "
        "match_number, "
        "red_landed_count, "
        "red_parked_count, "
        "red_sampled_count, "
        "red_minerals_lander_count, "
        "red_minerals_depot_count, "
        "red_crater_partial_count, "
        "red_crater_complete_count, "
        "red_latched_count, "
        "blue_landed_count, "
        "blue_parked_count, "
        "blue_sampled_count, "
        "blue_minerals_lander_count, "
        "blue_minerals_depot_count, "
        "blue_crater_partial_count, "
        "blue_crater_complete_count, "
        "blue_latched_count) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    sql::PreparedStatement *prepared_statement = connection->prepareStatement(query);
        
    prepared_statement->setInt(1, score.tournament_id);
    prepared_statement->setInt(2, score.match_number);
    prepared_statement->setInt(3, score.red_landed_count);
    prepared_statement->setInt(4, score.red_parked_count);
    prepared_statement->setInt(5, score.red_sampled_count);
    prepared_statement->setInt(6, score.red_minerals_lander_count);
    prepared_statement->setInt(7, score.red_minerals_depot_count);
    prepared_statement->setInt(8, score.red_crater_partial_count);
    prepared_statement->setInt(9, score.red_crater_complete_count);
    prepared_statement->setInt(10, score.red_latched_count);
    prepared_statement->setInt(11, score.blue_landed_count);
    prepared_statement->setInt(12, score.blue_parked_count);
    prepared_statement->setInt(13, score.blue_sampled_count);
    prepared_statement->setInt(14, score.blue_minerals_lander_count);
    prepared_statement->setInt(15, score.blue_minerals_depot_count);
    prepared_statement->setInt(16, score.blue_crater_partial_count);
    prepared_statement->setInt(17, score.blue_crater_complete_count);
    prepared_statement->setInt(18, score.blue_latched_count);

    deleteScore(score);
    prepared_statement->executeUpdate();
    delete prepared_statement;
}

/*
* Deletes a score with a particular tournament_id and match_number.
* 
* @param tournament_id the numerical id associated with the tournament
* @param match_number the match whose score is to be deleted
*/
void Sql::deleteScore(Score score) {
    sql::Statement *statement = connection->createStatement();
    std::string query = "DELETE FROM scores WHERE tournament_id = " + std::to_string(score.tournament_id) + " AND match_number = " + std::to_string(score.match_number);
    statement->executeUpdate(query);

    delete statement;
}

/*
* Deletes all scores associated with a particular tournament.
*
* @param tournament_id the id of the tournament for which scores are to be deleted
*/
void Sql::deleteScoresFromTournament(int tournament_id) {
    sql::Statement *statement = connection->createStatement();
    std::string query = "DELETE FROM scores WHERE tournament_id = " + std::to_string(tournament_id);
    statement->executeUpdate(query);

    delete statement;
}