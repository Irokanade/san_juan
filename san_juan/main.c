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

#define strBuff 129

void titleScreen(void);
void sanJuanGame(int);

int main(int argc, const char * argv[]) {
    // insert code here...
    titleScreen();
    sanJuanGame(3);
    return 0;
}

void titleScreen() {
    printf("San juan\n");
    printf("Base game edition\n");
}

void sanJuanGame(int noOfPlayers) {
    player playerArr[noOfPlayers];
    deck mainDeck;
    deck discardDeck;
    size_t turnCounter = 0; //turn counter starts from 0
    int gameOver = 0;
    
    initDeck(&mainDeck, 110);
    initDeck(&discardDeck, 110);
    
    setDeck(&mainDeck);
    shuffleDeck(&mainDeck, 110);
        
    //print deck
    /*for(size_t i = 0; i < 110; i++) {
        printf("cardname: %u\n", mainDeck.cardArr[i].cardName);
        printf("cost: %u\n", mainDeck.cardArr[i].cost);
        printf("victory point: %u\n\n", mainDeck.cardArr[i].victoryPoint);
    }*/
    
    //initialize all players
    initAllPlayers(playerArr, noOfPlayers);
    
    for(size_t i = 0; i < noOfPlayers; i++) {
        //draw 4 cards as starting hand
        drawCard(&playerArr[i], 4, &mainDeck);
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
        printf("palyer %d is now the governor\n", governor);
        
        //set roles starting from govenor
        for(int i = 0; i < noOfPlayers; i++) {
            int playerArrIndex = governor+i;
            
            if(playerArrIndex >= 3) {
                playerArrIndex -= 3;
            }
            
            //need to choose role
            int roleChoice = -1;
            while(roleChoice < 0 || roleChoice > 3) {
                printf("player %d choose a role\n", playerArrIndex);
                for(int i = 0; i < 4; i++) {
                    if(roleMainDeck.roleArr[i] != nullRole) {
                        printf("%1d - %s\n", i, roleTypeStr[i]);
                    } else {
                        printf("%s already chosen\n", roleTypeStr[i]);
                    }
                }
                printf("choice: ");
                scanf("%d", &roleChoice);
                if(drawFromRoleDeck(&roleMainDeck, roleChoice) == -1) {
                    printf("%s already chosen >:[\n", roleTypeStr[roleChoice]);
                    puts("choose again");
                    roleChoice = -1;
                }
            }
            printf("you have choosen %s role\n", roleTypeStr[roleChoice]);
            setRole(&playerArr[playerArrIndex], roleChoice);
            //printf("playerArrIndex: %d\n", playerArrIndex);
            
            //first player turn start
            //builder role
            if(playerArr[playerArrIndex].currRole == builder) {
                //every body gets to build starting from the governor
                int playerBuildIndex = 0;
                int buildChoice = 0;
                printf("___player %d hand___\n", playerArrIndex);
                printPlayerHand(playerArr[playerArrIndex]);
                printf("__________________\n");
                printf("enter card to build (number choice): ");
                scanf("%d", &buildChoice);
                
                card newBuilding;
                initCard(&newBuilding);
                newBuilding = popCardFromHand(&playerArr[playerArrIndex], playerArr[playerArrIndex].hand[buildChoice].cardName);
            }
            
            
            //trader role
            
            //councillor role
            
            //prospector role
            drawCard(&playerArr[playerArrIndex], 1, &mainDeck);
        }
        
        //printf("governor: %d\n", governor);
        
        turnCounter++;
    }
    
    return;
}
