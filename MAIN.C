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
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <sys/statvfs.h>
#include <assert.h>
#include "VERSION.H"
#include "MAIN.H"
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
#define QUEUE_OK         0
#define QUEUE_EMPTY     -1
#define QUEUE_FULL      -2
#ifdef _WIN32
#define OS 1
#elif _WIN64
#define OS 1
#elif __APPLE__
#define OS 2
#elif __linux__
#define OS 0
#endif



struct statvfs file_stats;
struct utsname name;
struct sysinfo my_sysinfo;




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

Queue 
*queue_with_size(size_t size) {
  Queue *q = malloc(sizeof(Queue));
  size_t byte_size = sizeof(char) * size;
  q->data = malloc(byte_size);
  memset(q->data, 0, byte_size);
  q->alloc_size = size;
  q->size = 0;
  q->offset = 0;
  return q;
}

int requeue(Queue *q) {
  if(q->size == q->alloc_size)
      return -1;
  
  size_t used_byte_size = sizeof(char) * q->size;
  char buf[q->size];
  char *relevant_data_start = q->data + q->offset;
  memcpy(buf, relevant_data_start, used_byte_size);
  memcpy(q->data, buf, used_byte_size);
  q->offset = 0;
  return 0;
}

int 
enqueue(Queue *q, char val) {
  if(q->size >= q->alloc_size)
      return QUEUE_FULL;
  size_t write_index = q->size + q->offset;
  if(write_index >= q->alloc_size) {
      requeue(q); 
      write_index = q->size;
  }
  q->data[write_index] = val;
  q->size++;
  return QUEUE_OK;
}

int 
dequeue(Queue *q, char *out) {
  if(q->size < 1)
      return QUEUE_EMPTY;
  if(q->offset >= q->alloc_size)
      return QUEUE_EMPTY;
  char front = q->data[q->offset];
  q->offset++; 
  q->size--;   
  *out = front;
  
  return QUEUE_OK;
}

void
destroy_queue(Queue *q) {
  free(q->data);
  free(q);
}

void 
read_after_sequence(FILE *fp, const char *seq, char *buffer, size_t buffer_size) {
  size_t seq_size = strlen(seq);
  Queue *q = queue_with_size(3 * seq_size);
  int ch, error;
  char elem;
  while((ch = fgetc(fp)) != EOF) {
    if(q->size < seq_size) {
        enqueue(q, ch);
        continue;
    }
    assert(q->size == seq_size);   
    if(strncmp((char *) q->data + q->offset, seq, seq_size) == 0)
        break;
    
    error = dequeue(q, &elem);
    assert(error != QUEUE_EMPTY);  
    error = enqueue(q, ch);
    assert(error != QUEUE_FULL); 
  }
  destroy_queue(q);
  for(size_t i = 0; i < buffer_size; ++i) {
      if((ch = fgetc(fp)) == EOF)
          break;
      buffer[i] = ch;
  }
}

static char 
*get_os() {
  char format[100];
  uname(&name);
  FILE *fp = fopen("/etc/os-release", "r");
  char buf[64];
  char *os_name = buf, *end;
  read_after_sequence(fp, "PRETTY_NAME", buf, 64);
  fclose(fp);
  if(os_name[0] == '"' || os_name[0] == '\'')
      ++os_name;
  
  end = strchr(os_name, '\n');
  *end = 0;
  if((end = strchr(os_name, '"'))) *end = 0;
  else if((end = strchr(os_name, '\''))) *end = 0;
  return os_name;
}

static bool 
isSystemValid(char *os) {

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
