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
    producer = 1,
    trader = 2,
    councillor = 3,
    prospector = 4,
} roleType;

static char *roleTypeStr[] = {
    "builder",
    "producer",
    "trader",
    "councillor",
    "prospector"
};

typedef struct _tradingHouseTile {
    int indigoPlantPrice;
    int sugarMillPrice;
    int tobaccoStoragePrice;
    int coffeRoasterPrice;
    int silverSmelterPrice;
} tradingHouseTile;

typedef struct _tradingHouseTileDeck {
    tradingHouseTile tradingHouseTileArr[5]; //only 5 trading house tiles
} tradingHouseTileDeck;

typedef struct _roleDeck {
    roleType roleArr[5]; //maximum of 5 types of roles
} roleDeck;

typedef struct _player {
    card hand[110]; //just add extra to the buffer, need function to make sure hand size is 7 or 12 due to tower
    card buildingCardsArr[12]; //maximum of 12 buildings
    card goodsCardsArr[12]; //good has the same index as the production building that produced it
    roleType currRole;
    uint8_t currGov;
} player;

void initPlayer(player *player1);
void initAllPlayers(player *playerArr, uint8_t noOfPlayers);
int getEmptyHandIndex(player player1);
void printPlayerHand(player player1);
void drawCard(player *player1, size_t noOfCards, deck *deck1);
void discardCard(player *player1, deck *discardDeck, int index);
void setRole(player *player1, roleType newRole);
void setRoleDeck(roleDeck *roleDeck1);
int drawFromRoleDeck(roleDeck *roleDeck1, roleType drawRole); //returns -1 when search fails
card popCardFromHand(player *player1, building chooseBuilding); //take a choosen card from hand
int getEmptyBuildingSlotIndex(player player1);
int getHandSize(player player1);
int sufficientCostToBuild(player player1, card newBuilding);
int buildBuildings(player *player1, card newBuilding); //build buildings
void printPlayerBuildings(player player1);
int getOpenProductionSlots(player player1);
int getNoOfGoods(player player1);
void produceGoods(player *player1, int index, deck *mainDeck);
void setTradingHouseTileDeck(tradingHouseTileDeck *tradingHouseTileDeck1);
void rotateTradingHouseTileDeck(tradingHouseTileDeck *tradingHouseTileDeck1);
tradingHouseTile popFromTradingHouseDeck(tradingHouseTileDeck *tradingHouseTileDeck1);
int getGoodPrice(tradingHouseTile tradingHouseTile1, building productionBuilding);
card popGoodFromPlayer(player *player1, int index);
void sellGood(player *player1, deck *mainDeck, deck *discardDeck, tradingHouseTile tradingHouseTile1, building productionBuilding, int index);

#endif /* player_h */
