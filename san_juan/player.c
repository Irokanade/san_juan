//
//  player.c
//  san_juan
//
//  Created by michaelleong on 12/06/2021.
//

#include "player.h"

void cleanUpHand(card *cardArr) {
    //test print
    printf("Before\n");
    for(size_t i = 0; i < 6; i++) {
        printf("card[%zu]: %s\n", i, buildingStr[cardArr[i].cardName]);
    }
    
    //search for null card
    //then search not null card
    //then move all elements from the not null card
    //to the first null card position
    //then add the subsequent number of null cards at the back
    for(int i = 0; i < 110; i++) {
        int firstNullCardIndex = -1; //initialize to -1
        int firstNotNullCardIndex = -1; //initialize to -1
        if(cardArr[i].cardName == -1) {
            firstNullCardIndex = i;
            for(int j = i; j < 110; j++) {
                if(cardArr[j].cardName != -1) {
                    firstNotNullCardIndex = j;
                    break;
                }
            }
        }
        
        //move all elements from firstNotNullCardIndex to firstNullCardIndex
        //firstNotNullCardIndex must be found else don't move elements
        if(firstNotNullCardIndex != -1) {
            int k = firstNullCardIndex;
            for(int j = firstNotNullCardIndex; j < 110; j++) {
                cardArr[k] = cardArr[j];
                k++;
            }
            
            //fill the rest with null cards
            for(k = k; k < 110; k++) {
                initCard(&cardArr[k]);
            }
        }
    }
    
    //test print
    printf("After\n");
    for(size_t i = 0; i < 6; i++) {
        printf("card[%zu]: %s\n", i, buildingStr[cardArr[i].cardName]);
    }
    
    return;
}

void initPlayer(player *player1) {
    player1->currGov = 0;
    player1->currRole = -1;
    
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
        initCard(&player1->goods[i]);
    }
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
            printf("%d - %s\n", i, buildingStr[player1.hand[i].cardName]);
        }
    }
}

int getTopDeckIndex(deck deck1) {
    //find first null card from the deck
    //search index start from 0
    int index = 0;
    while(deck1.cardArr[index].cardName == -1) {
        index++;
        if(index == 110) {
            //search failed
            return -1;
        }
    }
    
    return index;
}

card popFromTopDeck(deck *deck1) {
    //get card from the top of the deck
    card result;
    int index = getTopDeckIndex(*deck1);
    
    initCard(&result);
    index = getTopDeckIndex(*deck1);
    
    if(index == -1) {
        //search failed return null card
        //deck is empty
        return result;
    }
    result = deck1->cardArr[index];
    initCard(&deck1->cardArr[index]);
    
    return result;
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
    for(int i = 0; i < 4; i++) {
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
            cleanUpHand(player1->hand);
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

int sufficientCostToBuild(player player1, card newBuilding) {
    int cost = 0; //cost to build
    int newSize = 0; //number of cards minus the card to build
    cost = newBuilding.cost;
    
    popCardFromHand(&player1, newBuilding.cardName);
    newSize = getHandSize(player1);
    
    if(newSize >= cost) {
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
