//
//  card.c
//  san_juan
//
//  Created by michaelleong on 12/06/2021.
//

#include "card.h"
#include <stdlib.h>
#include <string.h>

void cleanUpCardArr(card *cardArr) {
    //test print
    /*printf("Before\n");
    for(size_t i = 0; i < 6; i++) {
        printf("card[%zu]: %s\n", i, buildingStr[cardArr[i].cardName]);
    }*/
    
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
    /*printf("After\n");
    for(size_t i = 0; i < 6; i++) {
        printf("card[%zu]: %s\n", i, buildingStr[cardArr[i].cardName]);
    }*/
    
    return;
}

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
    memset(card1->cardDescription, 0, strBuff);
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
    
    //city hall, guild hall, palace, triumphal arch have no base vp
    if(card1->cardName >= cityHall && card1->cardName <= triumphalArch) {
        card1->victoryPoint = 0;
    }
    
    //set card description
    if(card1->cardName == indigoPlant) {
        strncpy(card1->cardDescription, "producer phase owner produces one indigo", strBuff);
    } else if(card1->cardName == sugarMill) {
        strncpy(card1->cardDescription, "producer phase owner produces one sugar", strBuff);
    } else if(card1->cardName == tobaccoStorage) {
        strncpy(card1->cardDescription, "producer phase owner produces one tabacco", strBuff);
    } else if(card1->cardName == coffeeRoaster) {
        strncpy(card1->cardDescription, "producer phase owner produces one coffee", strBuff);
    } else if(card1->cardName == silverSmelter) {
        strncpy(card1->cardDescription, "producer phase owner produces one silver", strBuff);
    } else if(card1->cardName == archive) {
        strncpy(card1->cardDescription, "councillor phase owner may discard hand cards in addtion to drawn cards", strBuff);
    } else if(card1->cardName == goldmine) {
        strncpy(card1->cardDescription, "prospector phase owner turns up 4 cards from the supply, if all have different building costs, he may add one to his hand", strBuff);
    } else if(card1->cardName == smithy) {
        strncpy(card1->cardDescription, "builder phase owner pays 1 card less when building a production building", strBuff);
    } else if(card1->cardName == blackMarket) {
        strncpy(card1->cardDescription, "builder phase owner may discard any 1 or 2 goods and reduce the building cost by 1 or 2 cards", strBuff);
    } else if(card1->cardName == crane) {
        strncpy(card1->cardDescription, "builder phase owner may build over one of his buildings(and pay the difference)", strBuff);
    } else if(card1->cardName == marketStand) {
        strncpy(card1->cardDescription, "trader phase owner takes 1 card from the supply when he sells at least 2 goods", strBuff);
    } else if(card1->cardName == poorHouse) {
        strncpy(card1->cardDescription, "builder phase owner takes 1 card from the supply if he has only 0 or 1 hand card after building", strBuff);
    } else if(card1->cardName == tradingHouse) {
        strncpy(card1->cardDescription, "trader phase owner may sell 1 additional good", strBuff);
    } else if(card1->cardName == well) {
        strncpy(card1->cardDescription, "producer phase owner takes 1 card from the supply when he produces at least 2 goods", strBuff);
    } else if(card1->cardName == aqueduct) {
        strncpy(card1->cardDescription, "producer phase owner produces 1 good more", strBuff);
    } else if(card1->cardName == carpenter) {
        strncpy(card1->cardDescription, "builder phase owner takes 1 card from supply after he builds a violet building", strBuff);
    } else if(card1->cardName == chapel) {
        strncpy(card1->cardDescription, "governor phase owner may place 1 hand card under his chapel(each worth 1 VP at game end)", strBuff);
    } else if(card1->cardName == statue) {
        strncpy(card1->cardDescription, "monument(no special function)", strBuff);
    } else if(card1->cardName == tower) {
        strncpy(card1->cardDescription, "governor phase owner may have up to 12 cards in his hand", strBuff);
    } else if(card1->cardName == marketHall) {
        strncpy(card1->cardDescription, "trader phase owner takes 1 card more for selling one good", strBuff);
    } else if(card1->cardName == prefecture) {
        strncpy(card1->cardDescription, "councillor phase owner keeps 1 card more from those drawn", strBuff);
    } else if(card1->cardName == quarry) {
        strncpy(card1->cardDescription, "builder phase owner pays 1 card less when building a violet building", strBuff);
    } else if(card1->cardName == victoryColumn) {
        strncpy(card1->cardDescription, "monument(no special function)", strBuff);
    } else if(card1->cardName == hero) {
        strncpy(card1->cardDescription, "monument(no special function)", strBuff);
    } else if(card1->cardName == library) {
        strncpy(card1->cardDescription, "all phases owner uses the privilege of his role twice", strBuff);
    } else if(card1->cardName == cityHall) {
        strncpy(card1->cardDescription, "at game end owner earns 1 additional victory point for each for his violet buildings", strBuff);
    } else if(card1->cardName == guildHall) {
        strncpy(card1->cardDescription, "at game end owner earns 2 additional victory points for each of his production buildings", strBuff);
    } else if(card1->cardName == palace) {
        strncpy(card1->cardDescription, "at game end owner earns additional victory point for every 4 victory points", strBuff);
    } else if(card1->cardName == triumphalArch) {
        strncpy(card1->cardDescription, "at game end owner earns an additional 4-6-8 victory points for 1-2-3 monuments", strBuff);
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

int getTopDeckIndex(deck deck1) {
    //find first non null card from the deck
    //search index start from 0
    int index = 0;
    while(deck1.cardArr[index].cardName == -1) {
        index++;
        if(index >= 110) {
            //search failed
            return -1;
        }
    }
    
    return index;
}

int getBottomDeckIndex(deck deck1) {
    //find the first null card from the deck
    //search index start from 0
    int index = 0;
    while(deck1.cardArr[index].cardName != -1) {
        index++;
        if(index >= 110) {
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
    
    cleanUpCardArr(deck1->cardArr);
    
    return result;
}

int addToDeck(deck *deck1, card newCard) {
    int index = getBottomDeckIndex(*deck1);
    
    if(index == -1) {
        //deck is full
        return -1;
    }
    
    deck1->cardArr[index] = newCard;
    cleanUpCardArr(deck1->cardArr);
    
    return 1;
}

int isProductionBuilding(card card1) {
    if(card1.cardName>=indigoPlant && card1.cardName<=silverSmelter) {
        return 1;
    }
    
    return 0;
}

int isVioletBuilding(card card1) {
    if(card1.cardName>=archive && card1.cardName<=triumphalArch) {
        return 1;
    }
    
    return 0;
}
