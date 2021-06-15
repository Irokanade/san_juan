//
//  player.h
//  san_juan
//
//  Created by michaelleong on 12/06/2021.
//

#ifndef player_h
#define player_h

#include <stdio.h>
#include "card.h"

typedef enum _roleType {
    nullRole = -1,
    builder = 0,
    trader = 1,
    councillor = 2,
    prospector = 3,
} roleType;

static char *roleTypeStr[] = {
    "builder",
    "trader",
    "councillor",
    "prospector"
};

typedef struct _roleDeck {
    roleType roleArr[4]; //maximum of 4 types of roles
} roleDeck;

typedef struct _player {
    card hand[110]; //just add extra to the buffer, need function to make sure hand size is 7 or 12 due to tower
    card buildingCardsArr[12]; //maximum of 12 buildings
    card goods[12]; //maximum of 12 goods 1 good per production building
    roleType currRole;
    uint8_t currGov;
} player;

void cleanUpHand(card *cardArr);
void initPlayer(player *player1);
void initAllPlayers(player *playerArr, uint8_t noOfPlayers);
int getEmptyHandIndex(player player1);
void printPlayerHand(player player1);
int getTopDeckIndex(deck deck1);
card popFromTopDeck(deck *deck1);
void drawCard(player *player1, size_t noOfCards, deck *deck1);
void setRole(player *player1, roleType newRole);
void setRoleDeck(roleDeck *roleDeck1);
int drawFromRoleDeck(roleDeck *roleDeck1, roleType drawRole); //returns -1 when search fails
card popCardFromHand(player *player1, building chooseBuilding); //take a choosen card from hand
int getEmptyBuildingSlotIndex(player player1);
int getHandSize(player player1);
int sufficientCostToBuild(player player1, card newBuilding);
int buildBuildings(player *player1, card newBuilding); //build buildings

#endif /* player_h */
