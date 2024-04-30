#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct itemNode itemNode;

struct itemNode {
    char id[16];
    unsigned short qty;
    char desc[31];
    itemNode *next;
};

int parseCommand(char* args);
char* parseId(char* args);
unsigned short parseQty(char* args);

int main() {
   char args[15] = "add FxcG312:23";
   int a = parseCommand(args);
   char* b = parseId(args);
   unsigned short c = parseQty(args);

   printf("%d %s %hu\n", a, b, c);
   return 0;
}

int parseCommand(char* args) {
   char *command = strtok(args, " \0");

   for (size_t i = 0; command[i]; ++i) {
      command[i] = tolower(command[i]);
   }

   if (strcmp(command, "quit") == 0)
      return 0;
   else if (strcmp(command, "add") == 0)
      return 1;
   else if (strcmp(command, "remove") == 0)
      return 2;
   else if (strcmp(command, "print") == 0)
      return 3;
   else {
      printf("Unrecognised command");
      return -1;
   }
}

char* parseId(char* args) {
   char* parsedId = strtok(NULL, ":");
   printf("%s\n",  parsedId);

   char* id = (char*)malloc(16);
   if(id == NULL) {
      perror("Memory allocation error, close some other applications");
      return NULL;
   }

   if (sscanf(parsedId, "%15[^:]", id) != 1) {
      free(id);
      return NULL;
   }

   return id;
}

unsigned short parseQty(char* args) {
   char* parsedQty = strtok(NULL, " \0");
   // printf("%s\n", parsedQty);

   unsigned short qty;
   if (sscanf(parsedQty, "%hu", &qty) != 1) {
      return -1;
   }

   return qty;
}

void printItems(itemNode *inventory) {
   printf("  %-32s %-8s %-15s\n", "DESC", "QTY", "ID");
   printf("-------------------------------  -------  ----------------\n");

   while (inventory != NULL) {
      printf("%-30s %8hu    %-15s\n", inventory -> desc, inventory -> qty, inventory -> id);
      inventory = inventory -> next;
   }
}
