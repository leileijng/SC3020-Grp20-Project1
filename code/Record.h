#ifndef RECORD_H
#define RECORD_H

#include <iostream>
#include <string>

class Record {
private:
    std::string GAME_DATE_EST;
    long long TEAM_ID_home;
    int PTS_home;
    double FG_PCT_home, FT_PCT_home, FG3_PCT_home;
    int AST_home, REB_home;
    int HOME_TEAM_WINS;

public:
    Record(std::string GAME_DATE_EST,
           long long TEAM_ID_home,
           int PTS_home,
           double FG_PCT_home, double FT_PCT_home, double FG3_PCT_home,
           int AST_home, int REB_home,
           int HOME_TEAM_WINS);

    // Getter and Setter for GAME_DATE_EST
    std::string getGameDateEst() const;
    void setGameDateEst(const std::string& value);

    // Getter and Setter for TEAM_ID_home
    long long getTeamIdHome() const;
    void setTeamIdHome(long long value);

    // Getter and Setter for PTS_home
    int getPtsHome() const;
    void setPtsHome(int value);

    // Getter and Setter for FG_PCT_home
    double getFgPctHome() const;
    void setFgPctHome(double value);

    // Getter and Setter for FT_PCT_home
    double getFtPctHome() const;
    void setFtPctHome(double value);

    // Getter and Setter for FG3_PCT_home
    double getFg3PctHome() const;
    void setFg3PctHome(double value);

    // Getter and Setter for AST_home
    int getAstHome() const;
    void setAstHome(int value);

    // Getter and Setter for REB_home
    int getRebHome() const;
    void setRebHome(int value);

    // Getter and Setter for HOME_TEAM_WINS
    int getHomeTeamWins() const;
    void setHomeTeamWins(int value);

    // Calculate the size of record in bytes
    int size();
};

#endif
