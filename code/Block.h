
#include <cstring>
#include <iostream>

// Represents a storage block containing various game statistics.
class Block
{
private:
    std::string gameDate;
    std::string teamIdHome;
    int pointsHome;
    float fieldGoalPercentageHome;
    float freeThrowPercentageHome;
    float threePointPercentageHome;
    int assistsHome;
    int reboundsHome;
    int homeTeamWins;

public:
    // Constructor to initialize all game statistics.
    Block(std::string gameDate,
          std::string teamIdHome,
          int pointsHome,
          float fieldGoalPercentageHome,
          float freeThrowPercentageHome,
          float threePointPercentageHome,
          int assistsHome,
          int reboundsHome,
          int homeTeamWins);

    // Display the game statistics.
    void displayStats();
};
