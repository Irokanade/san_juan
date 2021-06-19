//
//  card.h
//  san_juan
//
//  Created by michaelleong on 12/06/2021.
//

#pragma once

#include <stdio.h>
#include <stdint.h>

typedef enum _building {
    //represent nullCard
    nullCard = -1,
    //sort by production building, violet buildings then by cost
    //5 types of production buildings
    indigoPlant = 1, //1 cost
    sugarMill, //2 cost
    tobaccoStorage, //3 cost
    coffeeRoaster, //4 cost
    silverSmelter, //5 cost
    //24 violet buildings
    //1 cost
    archive,
    goldmine,
    smithy,
    //2 cost
    blackMarket,
    crane,
    marketStand,
    poorHouse,
    tradingHouse,
    well,
    //3 cost
    aqueduct,
    carpenter,
    chapel,
    statue,
    tower,
    //4 cost
    marketHall,
    prefecture,
    quarry,
    victoryColumn,
    //5 cost
    hero,
    library,
    //6 cost
    cityHall,
    guildHall,
    palace,
    triumphalArch,
} building;

static char *buildingStr[] = {
    "nullCard",
    "indigo plant",
    "sugar mill",
    "tobacco storage",
    "coffee roaster",
    "silver smelter",
    "archive",
    "goldmine",
    "smithy",
    "black market",
    "crane",
    "market stand",
    "poor house",
    "trading post",
    "well",
    "aquaduct",
    "carpenter",
    "chapel",
    "statue",
    "tower",
    "market hall",
    "prefecture",
    "quarry",
    "victory column",
    "hero",
    "library",
    "city hall",
    "guild hall",
    "palace",
    "triumphal arch"
};

//card class
typedef struct _card {
    building cardName;
    uint8_t cost;
    uint8_t victoryPoint;
} card;

typedef struct _deck {
    card cardArr[110]; //exactly 110 cards
} deck;

void cleanUpCardArr(card *cardArr);
void shuffleDeck(deck *array, size_t n);
void initCard(card *card1);
void setCard(card *card1);
void setCardArr(card *cardArr, size_t size, building setBuilding);
void initDeck(deck *deck1, size_t size);
void setDeck(deck *deck1);
int getTopDeckIndex(deck deck1);
card popFromTopDeck(deck *deck1);;
int addToDeck(deck *deck1, card newCard);
int isProductionBuilding(card card1);
int isVioletBuilding(card card1);
