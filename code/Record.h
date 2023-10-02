#ifndef RECORD_H
#define RECORD_H

#include <iostream>

class Record {
private:
    std::string GAME_DATE_EST;
    int TEAM_ID_home;
    int PTS_home;
    double FG_PCT_home, FT_PCT_home, FG3_PCT_home;
    int AST_home, REB_home;
    int HOME_TEAM_WINS;
    int SEQ_NUMBER;

public:
    Record(std::string GAME_DATE_EST,
    int TEAM_ID_home,
    int PTS_home,
    double FG_PCT_home, double FT_PCT_home, double FG3_PCT_home,
    int AST_home, int REB_home,
    int HOME_TEAM_WINS,
    int SEQ_NUMBER);

    //calculate the size of record in bytes
    int size();
};

#endif