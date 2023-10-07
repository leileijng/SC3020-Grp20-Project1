#include "Record.h"

// Constructor
Record::Record(std::string GAME_DATE_EST,
               long long TEAM_ID_home,
               short PTS_home,
               float FG_PCT_home, float FT_PCT_home, float FG3_PCT_home,
               short AST_home, short REB_home,
               bool HOME_TEAM_WINS, int count)
    : GAME_DATE_EST(GAME_DATE_EST), TEAM_ID_home(TEAM_ID_home), PTS_home(PTS_home),
      FG_PCT_home(FG_PCT_home), FT_PCT_home(FT_PCT_home), FG3_PCT_home(FG3_PCT_home),
      AST_home(AST_home), REB_home(REB_home),
      HOME_TEAM_WINS(HOME_TEAM_WINS), count(count) {}

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
short Record::getPtsHome() const {
    return PTS_home;
}

void Record::setPtsHome(short value) {
    PTS_home = value;
}

// Getter and Setter for FG_PCT_home
float Record::getFgPctHome() const {
    return FG_PCT_home;
}

void Record::setFgPctHome(float value) {
    FG_PCT_home = value;
}

// Getter and Setter for FT_PCT_home
float Record::getFtPctHome() const {
    return FT_PCT_home;
}

void Record::setFtPctHome(float value) {
    FT_PCT_home = value;
}

// Getter and Setter for FG3_PCT_home
float Record::getFg3PctHome() const {
    return FG3_PCT_home;
}

void Record::setFg3PctHome(float value) {
    FG3_PCT_home = value;
}

// Getter and Setter for AST_home
short Record::getAstHome() const {
    return AST_home;
}

void Record::setAstHome(short value) {
    AST_home = value;
}

// Getter and Setter for REB_home
short Record::getRebHome() const {
    return REB_home;
}

void Record::setRebHome(short value) {
    REB_home = value;
}

// Getter and Setter for HOME_TEAM_WINS
bool Record::getHomeTeamWins() const {
    return HOME_TEAM_WINS;
}

void Record::setHomeTeamWins(bool value) {
    HOME_TEAM_WINS = value;
}

// Getter and Setter for count
bool Record::getCount() const {
    return count;
}

void Record::setCount(int _count) {
    count = _count;
}

// Calculate the size of record in bytes
int Record::size(){
    return sizeof(GAME_DATE_EST) + sizeof(TEAM_ID_home) + sizeof(PTS_home) + sizeof(FG_PCT_home) + sizeof(FT_PCT_home) + sizeof(FG3_PCT_home) + sizeof(AST_home) + sizeof(REB_home) + sizeof(HOME_TEAM_WINS) + sizeof(count);
};

void Record::display() const {
    std::cout << "Game Date: " << GAME_DATE_EST << std::endl;
    std::cout << "Team ID (Home): " << TEAM_ID_home << std::endl;
    //std::cout << "Points (Home): " << PTS_home << std::endl;
    std::cout << "Field Goal % (Home): " << FG_PCT_home << std::endl;
    //std::cout << "Free Throw % (Home): " << FT_PCT_home << std::endl;
    //std::cout << "3-Point Field Goal % (Home): " << FG3_PCT_home << std::endl;
    //std::cout << "Assists (Home): " << AST_home << std::endl;
    //std::cout << "Rebounds (Home): " << REB_home << std::endl;
    //std::cout << "Home Team Wins: " << (HOME_TEAM_WINS ? "Yes" : "No") << std::endl;
    std::cout << "Count: " << count << std::endl;
}