#ifndef RECORD_H
#define RECORD_H

#include <iostream>
#include <string>

class Record {
private:
    std::string GAME_DATE_EST;
    long long TEAM_ID_home;
    short PTS_home;
    float FG_PCT_home, FT_PCT_home, FG3_PCT_home;
    short AST_home, REB_home;
    bool HOME_TEAM_WINS;

public:
    Record(std::string GAME_DATE_EST,
           long long TEAM_ID_home,
           short PTS_home,
           float FG_PCT_home, float FT_PCT_home, float FG3_PCT_home,
           short AST_home, short REB_home,
           bool HOME_TEAM_WINS);

    // Getter and Setter for GAME_DATE_EST
    std::string getGameDateEst() const;
    void setGameDateEst(const std::string& value);

    // Getter and Setter for TEAM_ID_home
    long long getTeamIdHome() const;
    void setTeamIdHome(long long value);

    // Getter and Setter for PTS_home
    short getPtsHome() const;
    void setPtsHome(short value);

    // Getter and Setter for FG_PCT_home
    float getFgPctHome() const;
    void setFgPctHome(float value);

    // Getter and Setter for FT_PCT_home
    float getFtPctHome() const;
    void setFtPctHome(float value);

    // Getter and Setter for FG3_PCT_home
    float getFg3PctHome() const;
    void setFg3PctHome(float value);

    // Getter and Setter for AST_home
    short getAstHome() const;
    void setAstHome(short value);

    // Getter and Setter for REB_home
    short getRebHome() const;
    void setRebHome(short value);

    // Getter and Setter for HOME_TEAM_WINS
    bool getHomeTeamWins() const;
    void setHomeTeamWins(bool value);

    // Calculate the size of record in bytes
    int size();
};

#endif
