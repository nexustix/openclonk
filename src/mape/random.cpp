/*
 * mape - C4 Landscape.txt editor
 *
 * Copyright (c) 2005-2009 Armin Burgmeier
 *
 * Portions might be copyrighted by other authors who have contributed
 * to OpenClonk.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * See isc_license.txt for full license and disclaimer.
 *
 * "Clonk" is a registered trademark of Matthes Bender.
 * See clonk_trademark_license.txt for full license.
 */

#define MAPE_COMPILING_CPP

// TODO: Move this to an extra file:

#include <C4Application.h>
#include <C4Console.h>
#include <C4FullScreen.h>
#include <C4Game.h>
#include <C4Network2.h>

C4Application Application;
C4Console     Console;
C4FullScreen  FullScreen;
C4Game        Game;
C4Network2    Network;


#include <stdlib.h>
#include <C4Include.h>
#include <C4Random.h>
#include "mape/random.h"

extern "C" {

void mape_random_seed(unsigned int seed)
{
	FixedRandom(seed);
}

} // extern "C"
