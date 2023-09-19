#include <bits/stdc++.h>
using namespace std;

// Storage block
class Block{
  private:
    string GAME_DATE_EST;
    string TEAM_ID_home;
    int PTS_home;
    float FG_PCT_home;
    float FT_PCT_home;
    float FG3_PCT_home;
    int AST_home;
    int REB_home;
    int HOME_TEAM_WINS;

  public:
    Block(string GAME_DATE_EST,
        string TEAM_ID_home,
        int PTS_home,
        float FG_PCT_home,
        float FT_PCT_home,
        float FG3_PCT_home,
        int AST_home,
        int REB_home,
        int HOME_TEAM_WINS);
    void display();
};