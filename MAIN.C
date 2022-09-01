/* MAIN.C - main code and functions */
/*
 *  FUSB  --  Fixed USB
 *  Copyright (C) 2021  Davide Usberti aka UsboKirishima
 *
 *  FUSB is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  FUSB is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with FUSB.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>

#include "VERSION.H"

////////////////////////////////////////////////////
//               FUNCTIONS                        //
////////////////////////////////////////////////////

void 
HELP() {
  unsigned char *helpCommands;
  printf("%s", helpCommands);
}

////////////////////////////////////////////////////
//                 MAIN                           //
////////////////////////////////////////////////////

int 
main(int argc, char *argv[]) {
  printf("%s\n%s", "Hello, World!", VERSION);
}
