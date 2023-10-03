#include "Block.h"

// Initialize all game statistics using member initializer lists.
Block::Block(std::string _gameDate,
             std::string _teamIdHome,
             int _pointsHome,
             float _fieldGoalPercentageHome,
             float _freeThrowPercentageHome,
             float _threePointPercentageHome,
             int _assistsHome,
             int _reboundsHome,
             int _homeTeamWins)
    : gameDate(_gameDate),
      teamIdHome(_teamIdHome),
      pointsHome(_pointsHome),
      fieldGoalPercentageHome(_fieldGoalPercentageHome),
      freeThrowPercentageHome(_freeThrowPercentageHome),
      threePointPercentageHome(_threePointPercentageHome),
      assistsHome(_assistsHome),
      reboundsHome(_reboundsHome),
      homeTeamWins(_homeTeamWins)
{
}

// Display the game statistics.
void Block::displayStats()
{
    std::cout << gameDate << " "
              << teamIdHome << " "
              << pointsHome << " "
              << fieldGoalPercentageHome << " "
              << freeThrowPercentageHome << " "
              << threePointPercentageHome << " "
              << assistsHome << " "
              << reboundsHome << " "
              << homeTeamWins << std::endl;
}
