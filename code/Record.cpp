#include <iostream>
#include "Record.h"

Record::Record(std::string GAME_DATE_EST,
    int TEAM_ID_home,
    int PTS_home,
    double FG_PCT_home, double FT_PCT_home, double FG3_PCT_home,
    int AST_home, int REB_home,
    int HOME_TEAM_WINS,
    int SEQ_NUMBER){
        this->GAME_DATE_EST = GAME_DATE_EST;
        this->TEAM_ID_home = TEAM_ID_home;
        this->PTS_home = PTS_home;
        this->FG_PCT_home = FG_PCT_home;
        this->FT_PCT_home = FT_PCT_home;
        this->FG3_PCT_home = FG3_PCT_home;
        this->AST_home = AST_home;
        this->REB_home = REB_home;
        this->HOME_TEAM_WINS = HOME_TEAM_WINS;
        this->SEQ_NUMBER = SEQ_NUMBER;
    };

int Record::size(){
    return sizeof(GAME_DATE_EST) + sizeof(TEAM_ID_home) + sizeof(PTS_home) + sizeof(FG_PCT_home) + sizeof(FT_PCT_home) + sizeof(FG3_PCT_home) + sizeof(AST_home) + sizeof(REB_home) + sizeof(HOME_TEAM_WINS) + sizeof(SEQ_NUMBER);
};