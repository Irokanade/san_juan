//
//  main.c
//  san_juan
//
//  Created by michaelleong on 19/05/2021.
//

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define strBuff 129

typedef enum _role {
    builder,
    trader,
    councillor,
    prospector,
} role;

typedef enum _building {
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

//card class
typedef struct _card {
    building cardName;
    uint8_t cost;
    uint8_t victoryPoint;
} card;

typedef struct _deck {
    card cardArr[110]; //exactly 110 cards
} deck;

typedef struct _player {
    card hand[110]; //just add extra to the buffer, need function to make sure hand size is 7 or 12 due to tower
    role currRole;
    uint8_t currGov;
} player;

void titleScreen(void);
void sanJuanGame(int);

int main(int argc, const char * argv[]) {
    // insert code here...
    titleScreen();
    
    return 0;
}

void titleScreen() {
    printf("San juan\n");
    printf("Base game edition\n");
}

void sanJuanGame(int noOfPlayers) {
    player playerArr[noOfPlayers];
    
    
}

void initCard(card card1) {
    //set all card cost and victory points
    switch (card1.cardName) {
        case indigoPlant:
            card1.cost = 1;
            card1.victoryPoint = 1;
            break;
        case sugarMill:
            card1.cost = 2;
            card1.victoryPoint = 1;
            break;
        case tobaccoStorage:
            card1.cost = 3;
            card1.victoryPoint = 2;
            break;
        case coffeeRoaster:
            card1.cost = 4;
            card1.victoryPoint = 2;
            break;
        case silverSmelter:
            card1.cost = 5;
            card1.victoryPoint = 3;
            break;
        case archive:
            card1.cost = 1;
            card1.victoryPoint = 1;
            break;
        case goldmine:
            card1.cost = 1;
            card1.victoryPoint = 1;
            break;
        case smithy:
            card1.cost = 1;
            card1.victoryPoint = 1;
            break;
        case blackMarket:
            card1.cost = 2;
            card1.victoryPoint = 1;
            break;
        case crane:
            card1.cost = 2;
            card1.victoryPoint = 1;
            break;
        case marketStand:
            card1.cost = 2;
            card1.victoryPoint = 1;
            break;
        case poorHouse:
            card1.cost = 2;
            card1.victoryPoint = 1;
            break;
        case tradingHouse:
            card1.cost = 2;
            card1.victoryPoint = 1;
            break;
        case well:
            card1.cost = 2;
            card1.victoryPoint = 1;
            break;
        case aqueduct:
            card1.cost = 3;
            card1.victoryPoint = 2;
            break;
        case carpenter:
            card1.cost = 3;
            card1.victoryPoint = 2;
            break;
        case chapel:
            card1.cost = 3;
            card1.victoryPoint = 2;
            break;
        case statue:
            card1.cost = 3;
            card1.victoryPoint = 2;
            break;
        case tower:
            card1.cost = 3;
            card1.victoryPoint = 2;
            break;
        case marketHall:
            card1.cost = 4;
            card1.victoryPoint = 3;
            break;
        default:
            break;
    }
    
    
}

void initPlayer(player player1) {
    player1.currGov = 0;
    player1.currRole = -1;
    for(size_t i = 0; i < 12; i++) {
        initCard(player1.hand[i]);
    }
}

void initDeck(deck deck1) {
    //8 indigo plants
    for(size_t i = 0; i < 110; i++) {
        
    }
}
