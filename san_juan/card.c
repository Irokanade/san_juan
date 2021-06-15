//
//  card.c
//  san_juan
//
//  Created by michaelleong on 12/06/2021.
//

#include "card.h"
#include <stdlib.h>

void shuffleDeck(deck *deck1, size_t n) {
    if (n > 1) {
        int i;
        for (i = 0; i < n - 1; i++) {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            card t = deck1->cardArr[j];
            deck1->cardArr[j] = deck1->cardArr[i];
            deck1->cardArr[i] = t;
        }
    }
}

void initCard(card *card1) {
    card1->cardName = -1;
    card1->cost = 0;
    card1->victoryPoint = 0;
}

void setCard(card *card1) {    
    //set cost and victory points of cards
    if(card1->cardName <= silverSmelter) {
        card1->cost = (uint8_t)card1->cardName;
        //set victory point
        if(card1->cardName <= sugarMill) {
            card1->victoryPoint = 1;
        } else if(card1->cardName <= tobaccoStorage) {
            card1->victoryPoint = 2;
        } else if(card1->cardName == silverSmelter) {
            card1->victoryPoint = 3;
        }
    } else if(card1->cardName <= smithy) {
        card1->cost = 1;
        card1->victoryPoint = 1;
    } else if(card1->cardName <= well) {
        card1->cost = 2;
        card1->victoryPoint = 1;
    } else if(card1->cardName <= tower) {
        card1->cost = 3;
        card1->victoryPoint = 2;
    } else if(card1->cardName <= victoryColumn) {
        card1->cost = 4;
        card1->victoryPoint = 2;
    } else if(card1->cardName <= library) {
        card1->cost = 5;
        card1->victoryPoint = 3;
    } else if(card1->cardName <= triumphalArch) {
        card1->cost = 6;
        card1->victoryPoint = 3;
    }
    
    //set monuments victory points
    if(card1->cardName == statue) {
        card1->victoryPoint = 3;
    } else if(card1->cardName == victoryColumn) {
        card1->victoryPoint = 4;
    } else if(card1->cardName == hero) {
        card1->victoryPoint =  5;
    }
}

void setCardArr(card *cardArr, size_t size, building setBuilding) {
    for(size_t i = 0; i < size; i++) {
        cardArr[i].cardName = setBuilding;
        setCard(&cardArr[i]);
    }
}

void initDeck(deck *deck1, size_t size) {
    for(size_t i = 0; i < size; i++) {
        initCard(&deck1->cardArr[i]);
    }
}

void setDeck(deck *deck1) {
    int deckIndex = 0;
    
    //10 indigo plants
    setCardArr(&deck1->cardArr[deckIndex], 10, indigoPlant);
    deckIndex+=10;
    
    //8 copies
    for(int i = sugarMill; i <= silverSmelter; i++) {
        setCardArr(&deck1->cardArr[deckIndex], 8, sugarMill);
        deckIndex+=8;
    }
    
    //3 copies
    for(int i = archive; i <= library; i++) {
        setCardArr(&deck1->cardArr[deckIndex], 3, i);
        deckIndex+=3;
    }
    
    //2 copies
    for(int i = cityHall; i <= triumphalArch; i++) {
        setCardArr(&deck1->cardArr[deckIndex], 2, i);
        deckIndex+=2;
    }
    
    //shuffle deck (shuffle it outside of this function)
    //shuffleDeck(deck1->cardArr, 110);
    
}
