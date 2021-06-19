//
//  violetCards.h
//  san_juan
//
//  Created by michaelleong on 17/06/2021.
//

#pragma once

#include <stdio.h>
#include "player.h"

//card skill functions
//archive is determined in the coucillor phase
void goldmineFunc(player *player1, deck *mainDeck, deck *discardDeck);
//smithy is determined in the sufficient cost function
//black market determined in the build phase
int craneSufficientCost(player player1, card newBuilding, int index);
void craneBuildOver(player *player1, card newBuilding, int index);
//market stad determined in the trader phase
//poor house determined at the end of build phase
//trading house determined at the start of the trader phase
//well determined at the end of producer phase
//aquaduct determined at the start of producer phase
//carpenter determined at the end of build phase
void chapelFunc(player *player1, int index, deck *discardDeck);
//tower determined in checkhandsize > 7
//market hall determined at the end of trader phase
//prefecture is determined in the councillor phase
//quarry determined in the sufficient cost function

