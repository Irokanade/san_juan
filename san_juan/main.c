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
        //set roles starting from govenor
        for(int i = 0; i < noOfPlayers; i++) {
            int playerArrIndex = governor+i;
            
            if(playerArrIndex >= noOfPlayers) {
                playerArrIndex -= noOfPlayers;
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
                roleChoice = rand()%5; //random choice from 0 to 4
                printf("bot player %d has choosen %s role\n", playerArrIndex, roleTypeStr[roleChoice]);
            }
            
            setRole(&playerArr[playerArrIndex], roleChoice);
            //printf("playerArrIndex: %d\n", playerArrIndex);
            
            //first player turn start
            //builder role
            if(playerArr[playerArrIndex].currRole == builder) {
                //everybody gets to build starting from the player with builder role
                int playerBuildIndex = playerArrIndex;
                for(int j = 0; j < noOfPlayers; j++) {
                    playerBuildIndex = playerArrIndex+j;
                    
                    if(playerBuildIndex >= noOfPlayers) {
                        playerBuildIndex -= noOfPlayers;
                    }
                    
                    int buildChoice = -1; //initialize to build nothing
                    //if player is not bot
                    if(!playerArr[playerBuildIndex].isBot) {
                        //let human player choose what to build
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
                    } else {
                        //bot builds the first card it could build from its hand
                        int botBuilds = 0;
                        for(int k = 0; k < 110; k++) {
                            if(sufficientCostToBuild(playerArr[playerBuildIndex], playerArr[playerBuildIndex].hand[k])) {
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
                        
                        while(discardCost > 0) {
                            int discardChoice = -1;
                            
                            //let human player choose what to discard
                            if(!playerArr[playerBuildIndex].isBot) {
                                printf("___choose cards to discard___\n");
                                printf("___cards to discard %d___\n", discardCost);
                                printPlayerHand(playerArr[playerBuildIndex]);
                                printf("__________________\n");
                                printf("enter card to discard: ");
                                scanf(" %d", &discardChoice);
                                discardCard(&playerArr[playerBuildIndex], &discardDeck, discardChoice);
                            } else {
                                //bot discards whatever it could discard
                                discardCard(&playerArr[playerBuildIndex], &discardDeck, 0);
                            }
                            discardCost--;
                        }
                        
                        //cost is paid then build the building
                        buildBuildings(&playerArr[playerBuildIndex], newBuilding);
                        
                        //print buildings
                        printf("___player %d buildings___\n", playerBuildIndex);
                        printPlayerBuildings(playerArr[playerBuildIndex]);
                        printf("__________________\n");
                        
                    } else {
                        printf("player %d does not build anything\n", playerBuildIndex);
                    }
                    
                }
                
            } else if(playerArr[playerArrIndex].currRole == producer) {
                //producer role
                //everybody gets to produce starting from the player with the producer role
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
                                    printf("bot player %d produces %s good\n", playerProduceIndex, buildingStr[playerArr[playerProduceIndex].hand[k].cardName]);
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
                        }
                        noOfGoodCanProduce--;
                    }
                    
                }
                
            } else if(playerArr[playerArrIndex].currRole == trader) {
                //trader role
                //get the price first
                tradingHouseTile currPrices;
                currPrices = popFromTradingHouseDeck(&tradingHouseMainDeck);
                
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
                        }
                        noOfGoodsCanTrade--;
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
                    while(noOfCardsToDiscard > 0) {
                        int discardCardsIndex = -1;
                        
                        //let human player choose what to discard
                        if(!playerArr[playerCouncillorIndex].isBot) {
                            printf("___choose cards to discard___\n");
                            printf("___cards to discard %d___\n", noOfCardsToDiscard);
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
