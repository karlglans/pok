#include "GameSim.h"


GameSim::GameSim()
{
}


GameSim::~GameSim()
{
}

void GameSim::setNumberOfPlayers(int){
}

void GameSim::setStartHand(Card*){
}
void GameSim::setOpenCards(Card*, int nCards){
}
//virtual void setNumberOfPlayers(int) = 0;
void GameSim::setNumberOfFolds(int){
}
void GameSim::setPhase(gamePhase){
}

float GameSim::getWinChance(){
	return 2.0f;
}
