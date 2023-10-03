#include "Record.h"

// Constructor
Record::Record(std::string GAME_DATE_EST,
               long long TEAM_ID_home,
               int PTS_home,
               double FG_PCT_home, double FT_PCT_home, double FG3_PCT_home,
               int AST_home, int REB_home,
               int HOME_TEAM_WINS)
    : GAME_DATE_EST(GAME_DATE_EST), TEAM_ID_home(TEAM_ID_home), PTS_home(PTS_home),
      FG_PCT_home(FG_PCT_home), FT_PCT_home(FT_PCT_home), FG3_PCT_home(FG3_PCT_home),
      AST_home(AST_home), REB_home(REB_home),
      HOME_TEAM_WINS(HOME_TEAM_WINS){}

// Getter and Setter for GAME_DATE_EST
std::string Record::getGameDateEst() const {
    return GAME_DATE_EST;
}

void Record::setGameDateEst(const std::string& value) {
    GAME_DATE_EST = value;
}

// Getter and Setter for TEAM_ID_home
long long Record::getTeamIdHome() const {
    return TEAM_ID_home;
}

void Record::setTeamIdHome(long long value) {
    TEAM_ID_home = value;
}

// Getter and Setter for PTS_home
int Record::getPtsHome() const {
    return PTS_home;
}

void Record::setPtsHome(int value) {
    PTS_home = value;
}

// Getter and Setter for FG_PCT_home
double Record::getFgPctHome() const {
    return FG_PCT_home;
}

void Record::setFgPctHome(double value) {
    FG_PCT_home = value;
}

// Getter and Setter for FT_PCT_home
double Record::getFtPctHome() const {
    return FT_PCT_home;
}

void Record::setFtPctHome(double value) {
    FT_PCT_home = value;
}

// Getter and Setter for FG3_PCT_home
double Record::getFg3PctHome() const {
    return FG3_PCT_home;
}

void Record::setFg3PctHome(double value) {
    FG3_PCT_home = value;
}

// Getter and Setter for AST_home
int Record::getAstHome() const {
    return AST_home;
}

void Record::setAstHome(int value) {
    AST_home = value;
}

// Getter and Setter for REB_home
int Record::getRebHome() const {
    return REB_home;
}

void Record::setRebHome(int value) {
    REB_home = value;
}

// Getter and Setter for HOME_TEAM_WINS
int Record::getHomeTeamWins() const {
    return HOME_TEAM_WINS;
}

void Record::setHomeTeamWins(int value) {
    HOME_TEAM_WINS = value;
}

// Calculate the size of record in bytes
int Record::size(){
    return sizeof(GAME_DATE_EST) + sizeof(TEAM_ID_home) + sizeof(PTS_home) + sizeof(FG_PCT_home) + sizeof(FT_PCT_home) + sizeof(FG3_PCT_home) + sizeof(AST_home) + sizeof(REB_home) + sizeof(HOME_TEAM_WINS);
};