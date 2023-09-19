#include <bits/stdc++.h>
#include "StorageBlock.h"

using namespace std;

Block::Block(string _GAME_DATE_EST,
        string _TEAM_ID_home,
        int _PTS_home,
        float _FG_PCT_home,
        float _FT_PCT_home,
        float _FG3_PCT_home,
        int _AST_home,
        int _REB_home,
        int _HOME_TEAM_WINS){
    GAME_DATE_EST = _GAME_DATE_EST;
    TEAM_ID_home = _TEAM_ID_home;
    PTS_home = _PTS_home;
    FG_PCT_home = _FG_PCT_home;
    FT_PCT_home = _FT_PCT_home;
    FG3_PCT_home = _FG3_PCT_home;
    AST_home = _AST_home;
    REB_home = _REB_home;
    HOME_TEAM_WINS = _HOME_TEAM_WINS;  
};

void Block::display(){
    cout << GAME_DATE_EST << " " << TEAM_ID_home << " " << PTS_home << " " << FG_PCT_home << " " << FT_PCT_home << " " << FG3_PCT_home << " " << AST_home << " " << REB_home << " " << HOME_TEAM_WINS << endl;
};