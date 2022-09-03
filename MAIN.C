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
#pragma warning (disable: 128)
#include <stdio.h>
#include <stdlib.h>
#include <sys/mount.h>
#include <signal.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <unistd.h>
#include <stddef.h>
#include <stdalign.h>
#include <regex.h>
#include <stdbool.h>
#include "VERSION.H"
#define DEV             true
#define LINUX           true
#define NAME            "FUSB "
#define LOGS_MODE       false
#undef  MIN
#define MIN(x, y)       ((x) < (y) ? (x) : (y))
#undef  MAX
#define MAX(x, y)       ((x) > (y) ? (x) : (y))
#define ISODD(x)        ((x) & 1)
#define ISBLANK(x)      ((x) == ' ' || (x) == '\t')
#define TOUPPER(ch)     (((ch) >= 'a' && (ch) <= 'z') ? ((ch) - 'a' + 'A') : (ch))
#define TOLOWER(ch)     (((ch) >= 'A' && (ch) <= 'Z') ? ((ch) - 'A' + 'a') : (ch))
#define ISUPPER_(ch)    ((ch) >= 'A' && (ch) <= 'Z')
#define ISLOWER_(ch)    ((ch) >= 'a' && (ch) <= 'z')
#define ALIGN_UP(x, A)  ((((x) + (A) - 1) / (A)) * (A))
#define CONCAT(x, y)    (strcat(x, y))
#define READLINE_MAX    256
#define FILTER          '/'
#define RFILTER         '\\'
#define CASE            ':'
#define MSGWAIT         '$'
#define SELECT          ' '
#define PREFIX          '-'
#define LATEST_VERSION  VERSION
#define LATEST_VERSION  VERSION
#define NOTIFY_COMMAND  "notify-send "
#define COLOR_CYAN      "\033[36m"
#define COLOR_GREEN     "\033[32m"
#define COLOR_RED       "\033[31m"
#define COLOR_MAGENTA   "\033[35m"
#define COLOR_ORANGE    "\033[33m"
#define COLOR_WHITE     "\033[37m"
#define COLOR_RESET     "\033[0m"
#ifdef _WIN32
#define OS 1
#elif _WIN64
#define OS 1
#elif __APPLE__
#define OS 2
#elif __linux__
#define OS 0
#endif



////////////////////////////////////////////////////
//               FUNCTIONS                        //
////////////////////////////////////////////////////

static void 
HELP() {
  unsigned char *helpCommands;
  printf("%s", helpCommands);
}

static void
MOUNT(char *disk, char *name) {

}

static void
LIST(void) {
  system("sudo fdisk -l");
}

static void 
NOTIFY(char description[]) {
  char cmd[] = "notify-send FUSB ";
  strcat(cmd, description);
  system(cmd);
}

void 
ERROR(char *body) {
  printf("%s[%sERROR%s] %s%s", COLOR_RED, COLOR_RESET, COLOR_RED, COLOR_RESET, body);
}

static void
parse_options(int argc, char **argv) {
  for(int i = 1; i < argc; i++) {
    char *arg = argv[i];

    if(arg[0] != PREFIX) {
      ERROR("Invalid Option.");
      exit(0);
    }

   
    switch(arg[i]) {
      case 'h': 
        HELP();
      break;
      default: 
        ERROR("Invalid Option.");
        HELP();
      break;
    }
  }
}

////////////////////////////////////////////////////
//                 MAIN                           //
////////////////////////////////////////////////////

int 
main(int argc, char **argv) {



  if(LINUX == true) {
    switch(OS) {
      case 0:
        break;
      case 1: 
        printf("\n%s", "Software cannot be ran on Windows, try on Linux");
        exit(0);
        return(0);
        break;
      case 2: 
        printf("\n%s", "Software cannot be ran on MacOS, try on Linux");
        exit(0);
        return(0);
        break;
      default: 
        printf("\n%s", "Invalid System");
        exit(0);
        return(0);
        break;
    }

    parse_options(argc, argv);
  }

}
