//
//  violetCards.c
//  san_juan
//
//  Created by michaelleong on 17/06/2021.
//

#include "violetCards.h"

void goldmineFunc(player *player1, deck *mainDeck, deck *discardDeck) {
    int originalHandSize = getHandSize(*player1); //index of the original cards
    drawCard(player1, 4, mainDeck);
    //print all 4 new cards to see if all different cost
    for(int i = 0; i < 4; i++) {
        printf("%d - %20s cost: %d vp: %d\n", originalHandSize+i, buildingStr[player1->hand[originalHandSize+i].cardName], player1->hand[originalHandSize+i].cost, player1->hand[originalHandSize+i].victoryPoint);
    }
    //compare if the 4 cards have different card cost
    int strikeGold = 1;
    for(int i = 0; i < 4-1; i++) {
        for(int j = i+1; j < 4; j++) {
            if(player1->hand[originalHandSize+i].cost == player1->hand[originalHandSize+j].cost) {
                strikeGold = 0;
                break;
            }
            if(!strikeGold) {
                break;
            }
        }
    }
    
    if(strikeGold) {
        //all cards have different card cost
        //player gets to pick one
        printf("You strike gold!!!\n");
        for(int j = 0; j < 3; j++) {
            int discardCardIndex = -1;
            if(!player1->isBot) {
                //let human player choose what to discard
                printf("Choose %d card(s) to discard\n", j);
                for(int i = 0; i < 4; i++) {
                    printf("%d - %20s cost: %d vp: %d\n", originalHandSize+i, buildingStr[player1->hand[originalHandSize+i].cardName], player1->hand[originalHandSize+i].cost, player1->hand[originalHandSize+i].victoryPoint);
                }
                printf("__________________\n");
                printf("enter card index to discard: ");
                scanf(" %d", &discardCardIndex);
            } else {
                //bot just discards whatever
                discardCardIndex = originalHandSize;
            }
            discardCard(player1, discardDeck, discardCardIndex);
        }
        
    } else {
        printf("You didn't strike gold\n");
        //discard all cards drawn
        for(int i = 0; i < 4; i++) {
            discardCard(player1, discardDeck, originalHandSize+i);
        }
    }
}

int craneSufficientCost(player player1, card newBuilding, int index) {
    if(sufficientCostToBuild(player1, newBuilding)) {
        return 1;
    } else {
        int difference = cardsRemainingToBuild(player1, newBuilding);
        int oldBuildingCost = player1.buildingCardsArr[index].cost;
        difference += oldBuildingCost;
        
        if(difference >= 0) {
            return 1;
        }
    }
    
    //not enough cost even with the old building discount
    return 0;
}

void craneBuildOver(player *player1, card newBuilding, int index) {
    initCard(&player1->buildingCardsArr[index]); //init the card before building over
    player1->buildingCardsArr[index] = newBuilding;
}

void chapelFunc(player *player1, int index, deck *discardDeck) {
    discardCard(player1, discardDeck, index);
    //search for chapel card
    for(int i = 0; i < 12; i++) {
        if(player1->buildingCardsArr[i].cardName == chapel) {
            player1->buildingCardsArr[i].victoryPoint++;
            break;
        }
    }
}
