//
//  player.c
//  san_juan
//
//  Created by michaelleong on 12/06/2021.
//

#include "player.h"

void initPlayer(player *player1) {
    player1->currGov = 0;
    player1->currRole = -1;
    player1->isBot = 0;
    
    //init hand
    for(size_t i = 0; i < 110; i++) {
        initCard(&player1->hand[i]);
    }
    
    //init buildings
    for(size_t i = 0; i < 12; i++) {
        initCard(&player1->buildingCardsArr[i]);
    }
    
    //init goods
    for(size_t i = 0; i < 12; i++) {
        initCard(&player1->goodsCardsArr[i]);
    }
}

void setPlayerBot(player *player1) {
    player1->isBot = 1; //set isBot to true
}

void initAllPlayers(player *playerArr, uint8_t noOfPlayers) {
    for(uint8_t i = 0; i < noOfPlayers; i++) {
        initPlayer(&playerArr[i]);
    }
}

int getEmptyHandIndex(player player1) {
    //find player first empty hand
    //search index start from 0
    int index = 0;
    for(index = 0; index < 110; index++) {
        if(player1.hand[index].cardName == -1) {
            return index;
        }
    }
    
    //search failed
    return -1;
}

void printPlayerHand(player player1) {
    for(int i = 0; i < 110; i++) {
        if(player1.hand[i].cardName != -1) {
            //don't print null cards
            printf("%d - %20s cost: %d vp: %d %s\n", i, buildingStr[player1.hand[i].cardName], player1.hand[i].cost, player1.hand[i].victoryPoint, player1.hand[i].cardDescription);
        }
    }
}

void drawCard(player *player1, size_t noOfCards, deck *deck1) {
    int handIndex = 0;
    card topCard;
    initCard(&topCard);
    
    for(size_t i = 0; i < noOfCards; i++) {
        handIndex = getEmptyHandIndex(*player1);
        topCard = popFromTopDeck(deck1);
        player1->hand[handIndex] = topCard;
    }
    
    cleanUpCardArr(player1->hand);
    cleanUpCardArr(deck1->cardArr);
}

void discardCard(player *player1, deck *discardDeck, int index) {
    card temp = popCardFromHand(player1, player1->hand[index].cardName);
    addToDeck(discardDeck, temp);
}

void setRole(player *player1, roleType newRole) {
    player1->currRole = newRole;
}

void setRoleDeck(roleDeck *roleDeck1) {
    //set the role deck so all roles can be chosen again
    int roleDeckIndex = 0;
    for(roleType i = builder; i <= prospector; i++) {
        roleDeck1->roleArr[roleDeckIndex] = i;
        roleDeckIndex++;
    }
}

int drawFromRoleDeck(roleDeck *roleDeck1, roleType drawRole) {
    for(int i = 0; i < 5; i++) {
        if(roleDeck1->roleArr[i] == drawRole) {
            roleDeck1->roleArr[i] = nullRole;
            return 1;
        }
    }
    
    //search failed
    return -1;
}

card popCardFromHand(player *player1, building choosenBuilding) {
    card result;
    initCard(&result);
    
    for(size_t i = 0; i < 110; i++) {
        if(player1->hand[i].cardName == choosenBuilding) {
            result = player1->hand[i];
            initCard(&player1->hand[i]);
            cleanUpCardArr(player1->hand);
            break;
        }
    }
    
    return result;
}

int getEmptyBuildingSlotIndex(player player1) {
    for(int i = 0; i < 12; i++) {
        if(player1.buildingCardsArr[i].cardName == -1) {
            return i;
        }
    }
    
    //search failed
    return -1;
}

int getHandSize(player player1) {
    int size = 0;
    for(size_t i = 0; i < 110; i++) {
        if(player1.hand[i].cardName != -1) {
            size++;
        }
    }
    
    return size; //return handsize
}

int cardsRemainingToBuild(player player1, card newBuilding) {
    int cost = 0; //cost to build
    int newSize = 0; //number of cards minus the card to build
    cost = newBuilding.cost;
    int difference = 0;
    
    //check if player is builder role
    //builder role pay 1 card less
    if(player1.currRole == builder) {
        cost--;
        //library reduces building cost by 1 if player is builder
        if(searchPlayerBuilding(player1, library)) {
            cost--;
        }
    }
    
    //smithy card reduces production building cost by 1
    if(searchPlayerBuilding(player1, smithy)) {
        if(isProductionBuilding(newBuilding)) {
            cost--;
        }
    }
    
    //quarry reduces violet buildings cost by 1
    if(searchPlayerBuilding(player1, quarry)) {
        if(isVioletBuilding(newBuilding)) {
            cost--;
        }
    }
    
    
    
    popCardFromHand(&player1, newBuilding.cardName);
    newSize = getHandSize(player1);
    
    difference = newSize - cost;
    return difference; //if difference >= 0 then sufficient cost
}

int sufficientCostToBuild(player player1, card newBuilding) {
    if(cardsRemainingToBuild(player1, newBuilding) >= 0) {
        return 1;
    }
    
    //cards not enough no build
    return -1;
}

int buildBuildings(player *player1, card newBuilding) {
    int index = 0;
    index = getEmptyBuildingSlotIndex(*player1);
    
    if(index == -1) {
        //no empty building slots
        return -1;
    }
    
    player1->buildingCardsArr[index] = newBuilding;
    
    return 1;
}

void printPlayerBuildings(player player1) {
    for(int i = 0; i < 12; i++) {
        if(player1.buildingCardsArr[i].cardName == -1) {
            return;
        } else {
            printf("%d - %20s cost: %d vp: %d %s\n", i, buildingStr[player1.buildingCardsArr[i].cardName], player1.buildingCardsArr[i].cost, player1.buildingCardsArr[i].victoryPoint, player1.buildingCardsArr[i].cardDescription);
        }
    }
}

int getOpenProductionSlots(player player1) {
    //counts the number of production slots
    //number of production buildings without goods
    int count = 0;
    
    for(size_t i = 0; i < 12; i++) {
        if(isProductionBuilding(player1.buildingCardsArr[i]) && (player1.goodsCardsArr[i].cardName==-1)) {
            count++;
        }
    }
    return count;
}

int getNoOfGoods(player player1) {
    int count = 0;
    
    for(int i = 0; i < 12; i++) {
        if(player1.goodsCardsArr[i].cardName != 1) {
            count++;
        }
    }
    return count;
}

void produceGoods(player *player1, int index, deck *mainDeck) {
    //this function does not deal with producer role
    //does not take empty production slots into account
    if(!isProductionBuilding(player1->buildingCardsArr[index])) {
        //do nothing if parameter card is not a production building
        return;
    }
    
    //else produce good
    card goodCard;
    initCard(&goodCard);
    goodCard = popFromTopDeck(mainDeck);
    player1->goodsCardsArr[index] = goodCard;
}

void setTradingHouseTileDeck(tradingHouseTileDeck *tradingHouseTileDeck1) {
    
    //tile 0
    tradingHouseTileDeck1->tradingHouseTileArr[0].indigoPlantPrice = 1;
    tradingHouseTileDeck1->tradingHouseTileArr[0].sugarMillPrice = 1;
    tradingHouseTileDeck1->tradingHouseTileArr[0].tobaccoStoragePrice = 1;
    tradingHouseTileDeck1->tradingHouseTileArr[0].coffeRoasterPrice = 2;
    tradingHouseTileDeck1->tradingHouseTileArr[0].silverSmelterPrice = 2;
    
    //tile 1
    tradingHouseTileDeck1->tradingHouseTileArr[1].indigoPlantPrice = 1;
    tradingHouseTileDeck1->tradingHouseTileArr[1].sugarMillPrice = 1;
    tradingHouseTileDeck1->tradingHouseTileArr[1].tobaccoStoragePrice = 2;
    tradingHouseTileDeck1->tradingHouseTileArr[1].coffeRoasterPrice = 2;
    tradingHouseTileDeck1->tradingHouseTileArr[1].silverSmelterPrice = 2;
    
    //tile 2
    tradingHouseTileDeck1->tradingHouseTileArr[2].indigoPlantPrice = 1;
    tradingHouseTileDeck1->tradingHouseTileArr[2].sugarMillPrice = 1;
    tradingHouseTileDeck1->tradingHouseTileArr[2].tobaccoStoragePrice = 2;
    tradingHouseTileDeck1->tradingHouseTileArr[2].coffeRoasterPrice = 2;
    tradingHouseTileDeck1->tradingHouseTileArr[2].silverSmelterPrice = 3;
    
    //tile 3
    tradingHouseTileDeck1->tradingHouseTileArr[3].indigoPlantPrice = 1;
    tradingHouseTileDeck1->tradingHouseTileArr[3].sugarMillPrice = 2;
    tradingHouseTileDeck1->tradingHouseTileArr[3].tobaccoStoragePrice = 2;
    tradingHouseTileDeck1->tradingHouseTileArr[3].coffeRoasterPrice = 2;
    tradingHouseTileDeck1->tradingHouseTileArr[3].silverSmelterPrice = 3;
    
    //tile 4
    tradingHouseTileDeck1->tradingHouseTileArr[4].indigoPlantPrice = 1;
    tradingHouseTileDeck1->tradingHouseTileArr[4].sugarMillPrice = 2;
    tradingHouseTileDeck1->tradingHouseTileArr[4].tobaccoStoragePrice = 2;
    tradingHouseTileDeck1->tradingHouseTileArr[4].coffeRoasterPrice = 3;
    tradingHouseTileDeck1->tradingHouseTileArr[4].silverSmelterPrice = 3;
}

void rotateTradingHouseTileDeck(tradingHouseTileDeck *tradingHouseTileDeck1) {
    tradingHouseTile temp = tradingHouseTileDeck1->tradingHouseTileArr[0];
    for(int i = 0; i < 4; i++) {
        //move everything up
        tradingHouseTileDeck1->tradingHouseTileArr[i] = tradingHouseTileDeck1->tradingHouseTileArr[i+1];
    }
    //put the top most card to the bottom
    tradingHouseTileDeck1->tradingHouseTileArr[4] = temp;
}

tradingHouseTile popFromTradingHouseDeck(tradingHouseTileDeck *tradingHouseTileDeck1) {
    tradingHouseTile result = tradingHouseTileDeck1->tradingHouseTileArr[0];
    rotateTradingHouseTileDeck(tradingHouseTileDeck1);
    return result;
}

int getGoodPrice(tradingHouseTile tradingHouseTile1, building productionBuilding) {
    //get price of good from the building it is produced from
    int price = 0;
    
    if(productionBuilding == indigoPlant) {
        price = tradingHouseTile1.indigoPlantPrice;
    } else if(productionBuilding == sugarMill) {
        price = tradingHouseTile1.sugarMillPrice;
    } else if(productionBuilding == tobaccoStorage) {
        price = tradingHouseTile1.tobaccoStoragePrice;
    } else if(productionBuilding == coffeeRoaster) {
        price = tradingHouseTile1.coffeRoasterPrice;
    } else if(productionBuilding == silverSmelter) {
        price = tradingHouseTile1.silverSmelterPrice;
    }
    
    //return 0 if failed (building parameter was not a production building)
    return price;
}

card popGoodFromPlayer(player *player1, int index) {
    card result;
    initCard(&result);
    
    result = player1->goodsCardsArr[index];
    initCard(&player1->goodsCardsArr[index]);
    return result;
}

void printPrices(tradingHouseTile tradingHouseTile1) {
    printf("%20s: %d\n", "indigo plant good price", tradingHouseTile1.indigoPlantPrice);
    printf("%20s: %d\n", "sugar mill good price", tradingHouseTile1.sugarMillPrice);
    printf("%20s: %d\n", "tabacco storage good price", tradingHouseTile1.tobaccoStoragePrice);
    printf("%20s: %d\n", "coffee roaster good price", tradingHouseTile1.coffeRoasterPrice);
    printf("%20s: %d\n", "silver smelter good price", tradingHouseTile1.silverSmelterPrice);
}

void discardGood(player *player1, int index, deck *discardDeck) {
    card discardedGood;
    initCard(&discardedGood);
    
    discardedGood = popGoodFromPlayer(player1, index);
    addToDeck(discardDeck, discardedGood);
}

void sellGood(player *player1, deck *mainDeck, deck *discardDeck, tradingHouseTile tradingHouseTile1, building productionBuilding, int index) {
    //sells only one good at a time
    //call this function multiple times to sell multiple goods
    int currPrice = getGoodPrice(tradingHouseTile1, productionBuilding);
    //reomve the good from the goods card array
    //add card to discard deck
    card temp;
    initCard(&temp);
    temp = popGoodFromPlayer(player1, index);
    addToDeck(discardDeck, temp);
    //draw as many cards as the price
    drawCard(player1, currPrice, mainDeck);
}

void printPlayerGoods(player player1) {
    for(int i = 0; i < 12; i++) {
        if(player1.goodsCardsArr[i].cardName != -1) {
            printf("%d - %s good\n", i, buildingStr[player1.buildingCardsArr[i].cardName]);
        }
    }
}

int getCurrTotalVP(player player1) {
    int totalVp = 0;
    for(int i = 0; i < 12; i++) {
        totalVp += player1.buildingCardsArr[i].victoryPoint;
    }
    
    return totalVp;
}

void printPlayerCurrTotalVP(player player1) {
    printf("current vp: %d\n", getCurrTotalVP(player1));
}

int searchPlayerBuilding(player player1, building searchBuilding) {
    int found = 0;
    for(int i = 0; i < 12; i++) {
        if(player1.buildingCardsArr[i].cardName == searchBuilding) {
            found = 1;
            break;
        }
    }
    
    return found;
}

int deckSize(deck deck1) {
    int size = 0;
    
    for(size_t i = 0; i < 110; i++) {
        if(deck1.cardArr[i].cardName != -1) {
            size++;
        }
    }
    
    return size;
}
