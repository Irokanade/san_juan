//
//  main.c
//  san_juan
//
//  Created by michaelleong on 19/05/2021.
//

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "player.h"
#include "violetCards.h"

void titleScreen(int *noOfPlayers, int *noOfBots);
void sanJuanGame(int noOfPlayers, int noOfBots);

int main(int argc, const char * argv[]) {
    int noOfPlayers = 0;
    int noOfBots = 0;
    titleScreen(&noOfPlayers, &noOfBots);
    sanJuanGame(noOfPlayers, noOfBots);
    return 0;
}

void titleScreen(int *noOfPlayers, int *noOfBots) {
    printf("San juan\n");
    printf("Base game edition\n");
    printf("Choose number of players (2-4): ");
    scanf(" %d", noOfPlayers);
    while(*noOfPlayers > 4 || *noOfPlayers < 2) {
        printf("There can be only minimum of 2 players or maximum of 4 players\n");
        printf("Choose number of players (2-4)\n");
        scanf(" %d", noOfPlayers);
    }
    printf("Choose number of bots (0-3): ");
    scanf(" %d", noOfBots);
    while(*noOfBots > 3 || *noOfBots < 0) {
        printf("There can only be minimum of 0 bots or maximum of 3 bots\n");
        printf("Choose number of bots (0-3): ");
        scanf(" %d", noOfBots);
    }
}

void sanJuanGame(int noOfPlayers, int noOfBots) {
    player playerArr[noOfPlayers];
    deck mainDeck;
    deck discardDeck;
    size_t turnCounter = 0; //turn counter starts from 0
    int gameOver = 0;
    tradingHouseTileDeck tradingHouseMainDeck;
    srand((unsigned int)time(0)); //init random
    
    initDeck(&mainDeck, 110);
    initDeck(&discardDeck, 110);
    
    setDeck(&mainDeck);
    shuffleDeck(&mainDeck, 110);
    setTradingHouseTileDeck(&tradingHouseMainDeck);
        
    //print deck
    /*for(size_t i = 0; i < 110; i++) {
        printf("cardname: %u\n", mainDeck.cardArr[i].cardName);
        printf("cost: %u\n", mainDeck.cardArr[i].cost);
        printf("victory point: %u\n\n", mainDeck.cardArr[i].victoryPoint);
    }*/
    
    //initialize all players
    initAllPlayers(playerArr, noOfPlayers);
    //set bots
    for(int i = 0; i < noOfBots; i++) {
        int botIndex = i+(noOfPlayers-noOfBots);
        setPlayerBot(&playerArr[botIndex]);
    }
    
    for(size_t i = 0; i < noOfPlayers; i++) {
        //draw 4 cards as starting hand
        drawCard(&playerArr[i], 4, &mainDeck);
        
        //set indigo plant as first building
        card startingIndigoPlant;
        initCard(&startingIndigoPlant);
        startingIndigoPlant.cardName = indigoPlant;
        setCard(&startingIndigoPlant);
        playerArr[i].buildingCardsArr[0] = startingIndigoPlant;
    }
    
    while(!gameOver) {
        //reset the role deck
        roleDeck roleMainDeck;
        setRoleDeck(&roleMainDeck);
        
        //reset all roles this turn
        for(size_t i = 0; i < noOfPlayers; i++) {
            setRole(&playerArr[i], nullRole);
        }
        
        int governor = turnCounter%noOfPlayers;
        printf("\n\n");
        printf("turn %zu\n", turnCounter);
        printf("player %d is now the governor\n", governor);
        printf("\n\n");
        
        //activate chapel before choosing roles
        for(int j = 0; j < noOfPlayers; j++) {
            if(searchPlayerBuilding(playerArr[j], chapel)) {
                if(!playerArr[j].isBot) {
                    //let player choose what to add to chapel
                    int cardToAddToChapel = -1;
                    printf("___choose card to add to chapel input -1 to not add cards___\n");
                    printPlayerHand(playerArr[j]);
                    printf("__________________\n");
                    scanf(" %d", &cardToAddToChapel);
                    if(cardToAddToChapel != -1) {
                        chapelFunc(&playerArr[j], cardToAddToChapel, &discardDeck);
                    }
                }
                //bot will not add hahaha
            }
        }
        
        //check handsize > 7
        for(int j = 0; j < noOfPlayers; j++) {
            if(getHandSize(playerArr[j]) > 7) {
                //tower allow handsize up to 12
                if(searchPlayerBuilding(playerArr[j], tower)) {
                    if(getHandSize(playerArr[j]) > 12) {
                        int noOfCardsToDiscard = getHandSize(playerArr[j]) - 12;
                        printf("hand size more than 12 choose cards to discard\n");
                        while(noOfCardsToDiscard > 0) {
                            int cardToDiscardIndex = -1;
                            if(!playerArr[j].isBot) {
                                //let human player choose what to discard
                                printf("___cards to discard %d___\n", noOfCardsToDiscard);
                                printPlayerHand(playerArr[j]);
                                printf("__________________\n");
                                printf("enter card to discard: ");
                                scanf(" %d", &cardToDiscardIndex);
                            } else {
                                //bot discards the first card
                                cardToDiscardIndex = 0;
                            }
                            discardCard(&playerArr[j], &discardDeck, cardToDiscardIndex);
                            noOfCardsToDiscard--;
                        }
                    }
                } else {
                    int noOfCardsToDiscard = getHandSize(playerArr[j]) - 7;
                    printf("hand size more than 7 choose cards to discard\n");
                    while(noOfCardsToDiscard > 0) {
                        int cardToDiscardIndex = -1;
                        if(!playerArr[j].isBot) {
                            //let human player choose what to discard
                            printf("___cards to discard %d___\n", noOfCardsToDiscard);
                            printPlayerHand(playerArr[j]);
                            printf("__________________\n");
                            printf("enter card to discard: ");
                            scanf(" %d", &cardToDiscardIndex);
                        } else {
                            //bot discards the first card
                            cardToDiscardIndex = 0;
                        }
                        discardCard(&playerArr[j], &discardDeck, cardToDiscardIndex);
                        noOfCardsToDiscard--;
                    }
                }
            }
        }
        
        //set roles starting from govenor
        for(int i = 0; i < noOfPlayers; i++) {
            int playerArrIndex = governor+i;
            
            if(playerArrIndex >= noOfPlayers) {
                playerArrIndex -= noOfPlayers;
            }
            
            //check deck
            if(deckSize(mainDeck) <= 0) {
                //if deck empty reset deck
                setDeck(&mainDeck);
            }
            
            //need to choose role
            int roleChoice = -1;
            //let player choose role
            if(!playerArr[playerArrIndex].isBot) {
                //if player print player hand and buildings and goods
                if(!playerArr[playerArrIndex].isBot) {
                    
                    //print all of players information
                    printf("______player %d hand______\n", playerArrIndex);
                    printPlayerHand(playerArr[playerArrIndex]);
                    printf("______player %d buildings ______\n", playerArrIndex);
                    printPlayerBuildings(playerArr[playerArrIndex]);
                    printf("______player %d goods______\n", playerArrIndex);
                    printPlayerGoods(playerArr[playerArrIndex]);
                    printf("______player %d vp______\n", playerArrIndex);
                    printPlayerCurrTotalVP(playerArr[playerArrIndex]);
                    printf("________________________\n");
                    
                    //print players competitors information (goods and buildings and hand size)
                    for(int j = 0; j < noOfPlayers; j++) {
                        int printPlayerInfoIndex = j+playerArrIndex;
                        if(printPlayerInfoIndex >= noOfPlayers) {
                            printPlayerInfoIndex -= noOfPlayers;
                        }
                        printf("\n");
                        printf("______player %d handsize______\n", printPlayerInfoIndex);
                        printf("hand size: %d\n", getHandSize(playerArr[printPlayerInfoIndex]));
                        printf("______player %d buildings ______\n", printPlayerInfoIndex);
                        printPlayerBuildings(playerArr[printPlayerInfoIndex]);
                        printf("______player %d goods______\n", printPlayerInfoIndex);
                        printPlayerGoods(playerArr[printPlayerInfoIndex]);
                        printf("______player %d vp______\n", printPlayerInfoIndex);
                        printPlayerCurrTotalVP(playerArr[printPlayerInfoIndex]);
                        printf("________________________\n");
                        printf("\n");
                    }
                }
                
                while(roleChoice < 0 || roleChoice > 4) {
                    printf("player %d choose a role\n", playerArrIndex);
                    for(int i = 0; i < 5; i++) {
                        if(roleMainDeck.roleArr[i] != nullRole) {
                            printf("%1d - %s\n", i, roleTypeStr[i]);
                        } else {
                            printf("%s already chosen\n", roleTypeStr[i]);
                        }
                    }
                    printf("choice: ");
                    scanf(" %d", &roleChoice);
                    if(drawFromRoleDeck(&roleMainDeck, roleChoice) == -1) {
                        printf("%s already chosen >:[\n", roleTypeStr[roleChoice]);
                        puts("choose again");
                        roleChoice = -1;
                    }
                }
                printf("you have choosen %s role\n", roleTypeStr[roleChoice]);
            } else {
                //choose random role for the bot
                do {
                    roleChoice = rand()%5; //random choice from 0 to 4
                } while(drawFromRoleDeck(&roleMainDeck, roleChoice) == -1);
                printf("bot player %d has choosen %s role\n", playerArrIndex, roleTypeStr[roleChoice]);
            }
            
            setRole(&playerArr[playerArrIndex], roleChoice);
            //printf("playerArrIndex: %d\n", playerArrIndex);
            
            //first player turn start
            //builder role
            if(playerArr[playerArrIndex].currRole == builder) {
                //everybody gets to build starting from the player with builder role
                int playerBuildIndex = playerArrIndex;
                
                //poor house variable
                int builtSmtgThisTurn = 0;
                
                for(int j = 0; j < noOfPlayers; j++) {
                    playerBuildIndex = playerArrIndex+j;
                    
                    if(playerBuildIndex >= noOfPlayers) {
                        playerBuildIndex -= noOfPlayers;
                    }
                    
                    int buildChoice = -1; //initialize to build nothing
                    int buildOverBuilding = 0;
                    //crane variables
                    int buildingToOverrideIndex = -1;
                    int buildOverChoice = -1;
                    //if player is not bot
                    if(!playerArr[playerBuildIndex].isBot) {
                        //let human player choose what to build
                        //crane lets you build over existing buildings
                        if(searchPlayerBuilding(playerArr[playerBuildIndex], crane)) {
                            printf("build over existing building? (1:yes, 0:no): \n");
                            scanf(" %d", &buildOverBuilding);
                        }
                        
                        if(buildOverBuilding) {
                            //choose buidling to override
                            printf("Choose building to build over\n");
                            printPlayerBuildings(playerArr[playerBuildIndex]);
                            printf("__________________\n");
                            scanf(" %d", &buildingToOverrideIndex);
                            
                            //choose card to build
                            //card cost is discounted from the override building
                            printf("___Choose card to build___\n");
                            for(int k = 0; k < 110; k++) {
                                if(craneSufficientCost(playerArr[playerBuildIndex], playerArr[playerBuildIndex].hand[k], buildingToOverrideIndex) && playerArr[playerBuildIndex].hand[k].cardName != -1) {
                                    printf("%d - %20s cost: %d vp: %d\n", k, buildingStr[playerArr[playerBuildIndex].hand[k].cardName], playerArr[playerBuildIndex].hand[k].cost, playerArr[playerBuildIndex].hand[k].victoryPoint);
                                }
                            }
                            printf("________________________\n");
                            printf("Enter index to build enter -1 to not build: ");
                            scanf(" %d\n", &buildOverChoice);
                        } else {
                            printf("player %d build\n", playerBuildIndex);
                            printf("___player %d hand___\n", playerBuildIndex);
                            for(int k = 0; k < 110; k++) {
                                if(playerArr[playerBuildIndex].hand[k].cardName == -1) {
                                    break;
                                } else if(sufficientCostToBuild(playerArr[playerBuildIndex], playerArr[playerBuildIndex].hand[k]) != -1) {
                                    printf("%d - %20s cost: %d vp: %d\n", k, buildingStr[playerArr[playerBuildIndex].hand[k].cardName], playerArr[playerBuildIndex].hand[k].cost, playerArr[playerBuildIndex].hand[k].victoryPoint);
                                } else {
                                    printf("not enoungh cards to build %20s cost: %d vp: %d\n", buildingStr[playerArr[playerBuildIndex].hand[k].cardName], playerArr[playerBuildIndex].hand[k].cost, playerArr[playerBuildIndex].hand[k].victoryPoint);
                                }
                            }
                            printf("__________________\n");
                            printf("enter card to build (enter -1 to not build): ");
                            scanf(" %d", &buildChoice);
                        }
                        
                    } else {
                        //bot builds the first card it could build from its hand
                        int botBuilds = 0;
                        for(int k = 0; k < 110; k++) {
                            if(sufficientCostToBuild(playerArr[playerBuildIndex], playerArr[playerBuildIndex].hand[k]) != -1) {
                                buildChoice = k;
                                botBuilds = 1;
                                printf("bot player %d builds %s\n", playerBuildIndex, buildingStr[playerArr[playerBuildIndex].hand[k].cardName]);
                                break;
                            }
                        }
                        if(!botBuilds) {
                            printf("bot player %d builds nothing\n", playerBuildIndex);
                            buildChoice = -1; //if not enough to build anything build nothing
                        }
                    }
                    
                    if(buildChoice != -1) {
                        //discard cards for cost
                        card newBuilding;
                        initCard(&newBuilding);
                        newBuilding = popCardFromHand(&playerArr[playerBuildIndex], playerArr[playerBuildIndex].hand[buildChoice].cardName);
                        int discardCost = newBuilding.cost;
                        if(playerArr[playerBuildIndex].currRole == builder) {
                            //builder role pay one card less to build
                            discardCost--;
                        }
                        
                        //black market can discard up to 2 goods
                        int noOfDiscardGoods = 2;
                        while(discardCost > 0) {
                            int discardChoice = -1;
                            int discardGoodChoice = -1;
                            //let human player choose what to discard
                            if(!playerArr[playerBuildIndex].isBot) {
                                printf("___choose cards to discard___\n");
                                printf("___cards to discard %d___\n", discardCost);
                                printPlayerHand(playerArr[playerBuildIndex]);
                                printf("__________________\n");
                                //check for black market
                                //pay up to 2 goods as cost to build
                                if(searchPlayerBuilding(playerArr[playerBuildIndex], blackMarket) && noOfDiscardGoods > 0) {
                                    printf("___choose goods to discard___\n");
                                    printPlayerGoods(playerArr[playerBuildIndex]);
                                    printf("__________________\n");
                                    printf("___input -1 to discard from hand___\n");
                                    printf("discard good index: ");
                                    scanf(" %d", &discardGoodChoice);
                                }
                                if(discardGoodChoice != -1) {
                                    //discard good
                                    discardGood(&playerArr[playerBuildIndex], discardChoice, &discardDeck);
                                    noOfDiscardGoods--;
                                } else {
                                    //discard card from hand
                                    printf("enter card to discard: ");
                                    scanf(" %d", &discardChoice);
                                    discardCard(&playerArr[playerBuildIndex], &discardDeck, discardChoice);
                                }
                            } else {
                                //bot discards whatever it could discard
                                discardCard(&playerArr[playerBuildIndex], &discardDeck, 0);
                            }
                            discardCost--;
                        }
                        
                        //cost is paid then build the building
                        buildBuildings(&playerArr[playerBuildIndex], newBuilding);
                        
                        //carpenter
                        if(searchPlayerBuilding(playerArr[playerBuildIndex], carpenter)) {
                            if(isVioletBuilding(newBuilding)) {
                                drawCard(&playerArr[playerBuildIndex], 1, &mainDeck);
                            }
                        }
                        
                        //print buildings
                        printf("___player %d buildings___\n", playerBuildIndex);
                        printPlayerBuildings(playerArr[playerBuildIndex]);
                        printf("__________________\n");
                        
                    } else if(buildOverChoice != -1) {
                        card newBuilding = popCardFromHand(&playerArr[playerBuildIndex], playerArr[playerBuildIndex].hand[buildOverChoice].cardName);
                        
                        int discardCost = newBuilding.cost;
                        if(playerArr[playerBuildIndex].currRole == builder) {
                            //builder role pay one card less to build
                            discardCost--;
                        }
                        
                        //black market can discard up to 2 goods
                        int noOfDiscardGoods = 2;
                        while(discardCost > 0) {
                            int discardChoice = -1;
                            int discardGoodChoice = -1;
                            //let human player choose what to discard
                            if(!playerArr[playerBuildIndex].isBot) {
                                printf("___choose cards to discard___\n");
                                printf("___cards to discard %d___\n", discardCost);
                                printPlayerHand(playerArr[playerBuildIndex]);
                                printf("__________________\n");
                                //check for black market
                                //pay up to 2 goods as cost to build
                                if(searchPlayerBuilding(playerArr[playerBuildIndex], blackMarket) && noOfDiscardGoods > 0) {
                                    printf("___choose goods to discard___\n");
                                    printPlayerGoods(playerArr[playerBuildIndex]);
                                    printf("__________________\n");
                                    printf("___input -1 to discard from hand___\n");
                                    printf("discard good index: ");
                                    scanf(" %d", &discardGoodChoice);
                                }
                                if(discardGoodChoice != -1) {
                                    //discard good
                                    discardGood(&playerArr[playerBuildIndex], discardChoice, &discardDeck);
                                    noOfDiscardGoods--;
                                } else {
                                    //discard card from hand
                                    printf("enter card to discard: ");
                                    scanf(" %d", &discardChoice);
                                    discardCard(&playerArr[playerBuildIndex], &discardDeck, discardChoice);
                                }
                            } else {
                                //bot discards whatever it could discard
                                discardCard(&playerArr[playerBuildIndex], &discardDeck, 0);
                            }
                            discardCost--;
                        }
                        
                        //cost is paid then build buildings
                        craneBuildOver(&playerArr[playerBuildIndex], newBuilding, buildingToOverrideIndex);
                        
                        //carpenter
                        if(searchPlayerBuilding(playerArr[playerBuildIndex], carpenter)) {
                            if(isVioletBuilding(newBuilding)) {
                                drawCard(&playerArr[playerBuildIndex], 1, &mainDeck);
                            }
                        }
                        
                        //print buildings
                        printf("___player %d buildings___\n", playerBuildIndex);
                        printPlayerBuildings(playerArr[playerBuildIndex]);
                        printf("__________________\n");
                    } else {
                        printf("player %d does not build anything\n", playerBuildIndex);
                    }
                    
                    //poor house
                    if(searchPlayerBuilding(playerArr[playerBuildIndex], poorHouse)) {
                        if(builtSmtgThisTurn && (getHandSize(playerArr[playerBuildIndex]) <= 1)) {
                            drawCard(&playerArr[playerBuildIndex], 1, &mainDeck);
                        }
                    }
                }
                
            } else if(playerArr[playerArrIndex].currRole == producer) {
                //producer role
                //everybody gets to produce starting from the player with the producer role
                
                //well variables
                int noOfGoodProduced = 0;
                
                int playerProduceIndex = playerArrIndex;
                for(int j = 0; j < noOfPlayers; j++) {
                    playerProduceIndex = playerArrIndex+j;
                    
                    if(playerProduceIndex >= noOfPlayers) {
                        playerProduceIndex -= noOfPlayers;
                    }
                    
                    int noOfGoodCanProduce = 1; //defaults to 1
                    //need to deal with producer role
                    if(playerArr[playerProduceIndex].currRole == producer) {
                        noOfGoodCanProduce = 2;
                        //library is producer role noOfGoodsCanProduce = 3
                        if(searchPlayerBuilding(playerArr[playerProduceIndex], library)) {
                            noOfGoodCanProduce++;
                        }
                    }
                    
                    //aquaduct allows to produce 1 more good
                    if(searchPlayerBuilding(playerArr[playerProduceIndex], aqueduct)) {
                        noOfGoodCanProduce++;
                    }
                    
                    while(noOfGoodCanProduce > 0) {
                        int produceIndex = -1; //index where good will be produced
                        printf("player %d produce\n", playerProduceIndex);
                        
                        //let human player choose what to produce
                        if(!playerArr[playerProduceIndex].isBot) {
                            printf("___player %d choose building to produce___\n", playerProduceIndex);
                            for(int k = 0; k < 12; k++) {
                                if(isProductionBuilding(playerArr[playerProduceIndex].buildingCardsArr[k])) {
                                    if(playerArr[playerProduceIndex].goodsCardsArr[k].cardName == -1) {
                                        //there is no good under the production building
                                        printf("%d - %20s cost: %d vp: %d\n", k, buildingStr[playerArr[playerProduceIndex].buildingCardsArr[k].cardName], playerArr[playerProduceIndex].buildingCardsArr[k].cost, playerArr[playerProduceIndex].buildingCardsArr[k].victoryPoint);
                                    } else {
                                        printf("already has good %20s cost: %d vp: %d\n", buildingStr[playerArr[playerProduceIndex].buildingCardsArr[k].cardName], playerArr[playerProduceIndex].buildingCardsArr[k].cost, playerArr[playerProduceIndex].buildingCardsArr[k].victoryPoint);
                                    }
                                }
                            }
                            printf("__________________\n");
                            printf("enter good to produce enter -1 to not produce good: ");
                            scanf(" %d", &produceIndex);
                        } else {
                            //bot produces whatever it could produce
                            int botProduces = 0;
                            for(int k = 0; k < 12; k++) {
                                if(playerArr[playerProduceIndex].goodsCardsArr[k].cardName == -1) {
                                    produceIndex = k;
                                    botProduces = 1;
                                    printf("bot player %d produces %s good\n", playerProduceIndex, buildingStr[playerArr[playerProduceIndex].buildingCardsArr[k].cardName]);
                                    break;
                                }
                            }
                            if(!botProduces) {
                                printf("bot player %d does not produce anything\n", playerProduceIndex);
                                produceIndex = -1;
                            }
                        }
                        
                        
                        if(produceIndex != -1) {
                            produceGoods(&playerArr[playerProduceIndex], produceIndex, &mainDeck);
                            noOfGoodProduced++;
                        }
                        noOfGoodCanProduce--;
                    }
                    
                    //well allow player to draw one more card if you produce 2 or more goods
                    if(searchPlayerBuilding(playerArr[playerProduceIndex], well)) {
                        if(noOfGoodProduced >= 2) {
                            drawCard(&playerArr[playerProduceIndex], 1, &mainDeck);
                        }
                    }
                }
                
            } else if(playerArr[playerArrIndex].currRole == trader) {
                //trader role
                //get the price first
                tradingHouseTile currPrices;
                currPrices = popFromTradingHouseDeck(&tradingHouseMainDeck);
                
                //market stand and market hall variables
                int noOfGoodsSold = 0;
                
                //everybody gets to trade starting from the player who chose the trader role
                int playerTraderIndex = playerArrIndex;
                for(int j = 0; j < noOfPlayers; j++) {
                    playerTraderIndex = playerArrIndex+j;
                    
                    if(playerTraderIndex >= noOfPlayers) {
                        playerTraderIndex -= noOfPlayers;
                    }
                    
                    int noOfGoodsCanTrade = 1; //defaults to 1
                    //need to deal with trader role
                    if(playerArr[playerTraderIndex].currRole == trader) {
                        noOfGoodsCanTrade = 2;
                        //library allows to trader one more good if trader
                        if(searchPlayerBuilding(playerArr[playerTraderIndex], library)) {
                            noOfGoodsCanTrade++;
                        }
                    }
                    
                    //trading house allow you trade one more good
                    if(searchPlayerBuilding(playerArr[playerTraderIndex], tradingHouse)) {
                        noOfGoodsCanTrade++;
                    }
                    
                    while(noOfGoodsCanTrade) {
                        int goodToSellIndex = -1; //index of which good to be sold
                        printf("player %d sell\n", playerTraderIndex);
                        
                        //let human player choose what to sell
                        if(!playerArr[playerTraderIndex].isBot) {
                            printf("___player %d choose good to sell___\n", playerTraderIndex);
                            for(int k = 0; k < 12; k++) {
                                if(isProductionBuilding(playerArr[playerTraderIndex].buildingCardsArr[k]) && playerArr[playerTraderIndex].goodsCardsArr[k].cardName != -1) {
                                    //there must be a production building with a good
                                    printf("%d - %20s sell price: %d\n", k, buildingStr[playerArr[playerTraderIndex].buildingCardsArr[k].cardName], getGoodPrice(currPrices, playerArr[playerTraderIndex].buildingCardsArr[k].cardName));
                                }
                            }
                            printf("__________________\n");
                            printf("enter index good to sell input -1 to not sell anything: ");
                            scanf("%d", &goodToSellIndex);
                        } else {
                            //bot trades whatever it could trade
                            int botSell = 0;
                            for(int k = 0; k < 12; k++) {
                                if(isProductionBuilding(playerArr[playerTraderIndex].buildingCardsArr[k]) && playerArr[playerTraderIndex].goodsCardsArr[k].cardName != -1) {
                                    //there must be a production building with a good
                                    goodToSellIndex = k;
                                    botSell = 1;
                                    printf("bot player %d sells %s good\n", playerTraderIndex, buildingStr[playerArr[playerTraderIndex].hand[k].cardName]);
                                    break;
                                }
                            }
                            if(!botSell) {
                                goodToSellIndex = -1;
                                printf("bot player %d does not sell anything\n", playerTraderIndex);
                            }
                        }
                        
                        
                        if(goodToSellIndex != -1) {
                            sellGood(&playerArr[playerTraderIndex], &mainDeck, &discardDeck, currPrices, playerArr[playerTraderIndex].buildingCardsArr[goodToSellIndex].cardName, goodToSellIndex);
                            noOfGoodsSold++;
                        }
                        noOfGoodsCanTrade--;
                    }
                    
                    //if sold more than 2 goods activate market stand
                    if(searchPlayerBuilding(playerArr[playerTraderIndex], marketStand)) {
                        if(noOfGoodsSold >= 2) {
                            drawCard(&playerArr[playerTraderIndex], 1, &mainDeck);
                        }
                    }
                    
                    //if sold at least 1 good draw card
                    if(searchPlayerBuilding(playerArr[playerTraderIndex], marketHall)) {
                        if(noOfGoodsSold >= 1) {
                            drawCard(&playerArr[playerTraderIndex], 1, &mainDeck);
                        }
                    }
                }
            } else if(playerArr[playerArrIndex].currRole == councillor) {
                //councillor role
                //everybody draws 2 cards and discards 1 staring from the player who chose councillor role
                //The player with councillor role draws 5 cards discards 4 cards
                int playerCouncillorIndex = playerArrIndex;
                for(int j = 0; j < noOfPlayers; j++) {
                    playerCouncillorIndex = playerArrIndex+j;
                    
                    if(playerCouncillorIndex >= noOfPlayers) {
                        playerCouncillorIndex -= noOfPlayers;
                    }
                    
                    int noOfCardsToDraw = 2; //defaults to 2
                    //deal with councillor role
                    if(playerArr[playerCouncillorIndex].currRole == councillor) {
                        noOfCardsToDraw = 5;
                        //library make player draw 8 cards instead
                        if(searchPlayerBuilding(playerArr[playerCouncillorIndex], library)) {
                            noOfCardsToDraw = 8;
                        }
                    }
                    //get the index of the original hand size
                    int originalHandSize = getHandSize(playerArr[playerCouncillorIndex]);
                    //draw the cards first
                    drawCard(&playerArr[playerCouncillorIndex], noOfCardsToDraw, &mainDeck);
                    //then print the cards
                    //printPlayerHand(playerArr[playerCouncillorIndex]);
                    //then you discard cards
                    int noOfCardsToDiscard = 0;
                    noOfCardsToDiscard = noOfCardsToDraw - 1; //discard until one card left
                    //deal with prefecture
                    if(searchPlayerBuilding(playerArr[playerCouncillorIndex], prefecture)) {
                        //keep 2 cards instead of 1
                        noOfCardsToDiscard = noOfCardsToDiscard - 2;
                    }
                    while(noOfCardsToDiscard > 0) {
                        int discardCardsIndex = -1;
                        
                        //let human player choose what to discard
                        if(!playerArr[playerCouncillorIndex].isBot) {
                            printf("___choose cards to discard___\n");
                            printf("___cards to discard %d___\n", noOfCardsToDiscard);
                            //deal with archive
                            if(searchPlayerBuilding(playerArr[playerCouncillorIndex], archive)) {
                                //you may also discard cards from your original hand
                                noOfCardsToDraw = originalHandSize+noOfCardsToDraw;
                                originalHandSize = 0; //print the whole hand plus the newly drawn ones
                            }
                            
                            for(int k = 0; k < noOfCardsToDraw; k++) {
                                int newCardIndex = originalHandSize + k;
                                if(playerArr[playerCouncillorIndex].hand[newCardIndex].cardName != -1) {
                                    printf("%d - %20s cost: %d vp: %d\n", newCardIndex, buildingStr[playerArr[playerCouncillorIndex].hand[newCardIndex].cardName], playerArr[playerCouncillorIndex].hand[newCardIndex].cost, playerArr[playerCouncillorIndex].hand[newCardIndex].victoryPoint);
                                }
                            }
                            printf("__________________\n");
                            printf("enter index of hand to discard: ");
                            scanf("%d", &discardCardsIndex);
                            discardCard(&playerArr[playerCouncillorIndex], &discardDeck, discardCardsIndex);
                        } else {
                            //bot discard the first card
                            discardCard(&playerArr[playerCouncillorIndex], &discardDeck, originalHandSize);
                        }
                        noOfCardsToDiscard--;
                    }
                }
            } else if(playerArr[playerArrIndex].currRole == prospector) {
                //prospector role
                drawCard(&playerArr[playerArrIndex], 1, &mainDeck);
                if(searchPlayerBuilding(playerArr[playerArrIndex], library)) {
                    //library draw one more card from prospector role
                    drawCard(&playerArr[playerArrIndex], 1, &mainDeck);
                }
                
                for(int k = 0; k < noOfPlayers; k++) {
                    if(searchPlayerBuilding(playerArr[k], goldmine)) {
                        goldmineFunc(&playerArr[k], &mainDeck, &discardDeck);
                    }
                }
            }
        }
        
        //game ends if any player has built 12 buildings
        for(size_t i = 0; i < noOfPlayers; i++) {
            if(playerArr[i].buildingCardsArr[11].cardName != -1) {
                //just check if the last building is not empty
                //since building are built from starting from index 0
                gameOver = 1;
                break;
            }
        }
        
        turnCounter++;
    }
    
    //game over count scores
    printf("___game over___\n");
    int highestScore = 0;
    int winnerIndex = -1;
    for(int i = 0; i < noOfPlayers; i++) {
        int totalVp = 0;
        totalVp = getCurrTotalVP(playerArr[i]);
        /*for(size_t j = 0; j < 12; j++) {
            if(playerArr[i].buildingCardsArr[j].cardName != -1) {
                totalVp += playerArr[i].buildingCardsArr[j].victoryPoint;
            }
        }*/
        
        //city hall
        //1 vp for each violet building
        if(searchPlayerBuilding(playerArr[i], cityHall)) {
            int noOfVioletBuildings = 0;
            for(int j = 0; j < 12; j++) {
                if(isVioletBuilding(playerArr[i].buildingCardsArr[j])) {
                    noOfVioletBuildings++;
                }
            }
            
            totalVp += noOfVioletBuildings;
        }
        
        //guild hall
        //2 vp for each production buildings
        if(searchPlayerBuilding(playerArr[i], cityHall)) {
            int noOfProductionBuildings = 0;
            for(int j = 0; j < 12; j++) {
                if(isProductionBuilding(playerArr[i].buildingCardsArr[j])) {
                    noOfProductionBuildings++;
                }
            }
            
            totalVp += (noOfProductionBuildings*2);
        }
        
        //triumphal arch
        if(searchPlayerBuilding(playerArr[i], triumphalArch)) {
            int noOfMonumets = 0;
            int triumphalArchVp = 0;
            if(searchPlayerBuilding(playerArr[i], statue)) {
                noOfMonumets++;
            }
            if(searchPlayerBuilding(playerArr[i], victoryColumn)) {
                noOfMonumets++;
            }
            if(searchPlayerBuilding(playerArr[i], hero)) {
                noOfMonumets++;
            }
            
            if(noOfMonumets >= 3) {
                triumphalArchVp = 8;
            } else if(noOfMonumets == 2) {
                triumphalArchVp = 6;
            } else if(noOfMonumets == 1) {
                triumphalArchVp = 4;
            }
            
            totalVp += triumphalArchVp;
        }
        
        //palace calculated last
        if(searchPlayerBuilding(playerArr[i], palace)) {
            int palaceVp = totalVp/4;
            totalVp += palaceVp;
        }
        
        
        printf("player %d total vp: %d\n", i, totalVp);
        //calculate winner by finding out the highest score
        if(totalVp >= highestScore) {
            highestScore = totalVp;
            winnerIndex = i;
        }
    }
    printf("___\\congrats/___\n");
    printf("player %d is the winner!!!\n", winnerIndex);
    
    return;
}
