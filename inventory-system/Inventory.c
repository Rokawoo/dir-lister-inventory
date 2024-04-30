/*******************************************************
 * Inventory.c - interactive utility that maintains a database
 *
 * Rokawoo
 * Nov 2023
 *
 * gcc (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0
 * 5.15.0-87-generic x86_64 GNU/Linux
 *******************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>

typedef struct itemNode itemNode;

struct itemNode {
    char id[16];
    unsigned short qty;
    char desc[31];
    itemNode *next;
};

// Read a data file (inv.dat) converting entries into itemNodes
// Pre: data file (inv.dat) exists and is in working directory
// Post: itemNodes for entries in data file and return head itemNode - itemNode 
itemNode* readDatabaseFile();

// Start interactive database and await user input
// Pre: itemNodes exist - itemNode
// Post: all user commands are handled and itemNodes are updated accordingly
void interactiveDatabase(itemNode* inventory);

// Read a string for a command and return its corresponding value
// Pre: string containing a command, id, and qty - char
// Post: command value - int
int parseCommand(char* args);

// Read a string for an ID and return that ID
// Pre: string containing a command, id, and qty - char
// Post: string containing an id - char
char* parseId(char* args);

// Read a string for a valid Qty (unsigned short and no letters) and return that qty
// Pre: string containing a command, id, and qty - char
// Post: unsigned short of qty - unsigned short
unsigned short parseQty(char* args);

// Add item qty to exisiting item
// Pre: item exists as itemNode - itemNode
// Post: itemNode of existing item will have its qty added to
void addItems(itemNode* inventory, char* id, unsigned short qty);

// Remove item qty from exisiting item
// Pre: item exists as itemNode - itemNode and has ample qty - unsigned short
// Post: itemNode of existing item will have its qty subtracted from
void removeItems(itemNode* inventory, char* id, unsigned short qty);

// Pretty print inventory to stdout
// Pre: itemNodes exist - itemNode
// Post: itemNode data will be printed to stdout
void printItems(const itemNode* inventory);

// Flush inventory nodes back to data file (inv.dat)
// Pre: itemNodes exist - itemNode
// Post: itemNodes are saved in data file (inv.dat)
void flushInventory(itemNode* inventory);

// Free itemNodes and return the memory to the heap
// Pre: itemNodes exist - itemNodes and quitAndFlush() is called
// Post: itemNodes are freed
void freeInventory(itemNode* inventory);

int main() {
   itemNode* headInventoryNode;
   headInventoryNode = readDatabaseFile();
   if(headInventoryNode == NULL) {return -1;}
   
   interactiveDatabase(headInventoryNode);
   flushInventory(headInventoryNode);
   freeInventory(headInventoryNode);

   return 0;
}

itemNode* readDatabaseFile() {
   FILE* file = fopen("inv.dat", "r");
   if (file == NULL) {
      perror("Error opening file, make sure 'inv.dat' is in working directory and accessible");
      return NULL;
   }

   itemNode* inventory = NULL;
   char line[64];

   while(fgets(line, sizeof(line), file) != NULL) {
      size_t len = strlen(line);
      while(len > 0 && (line[len - 1] == '\n')) {line[len - 1] = '\0';}

      char* id = strtok(line, ":");
      char* qtyStr = strtok(NULL, ":");
      char* desc = strtok(NULL, ":");
      
      if(id != NULL && qtyStr != NULL && desc != NULL) {
         unsigned short qty = atoi(qtyStr);

         itemNode* newNode = (itemNode*)malloc(sizeof(itemNode));
         if(newNode == NULL) {
            perror("Memory allocation error, close some other applications");
            fclose(file);
            return NULL;
         }

         strncpy(newNode -> id, id, 16);
         newNode -> id[15] = '\0';

         newNode -> qty = qty;

         strncpy(newNode -> desc, desc, 31);
         newNode -> desc[30] = '\0';

         newNode -> next = inventory;
         inventory = newNode;
      } else {
         perror("Error reading line from file, check file is correctly formatted");
      }
   }

   fclose(file);
   return inventory;
}

void interactiveDatabase(itemNode* inventory) {
   bool boolLoop = true;
   char* buff = NULL;
   size_t len = 0;
   int command;
   char* id = NULL;
   unsigned short qty;

   while(boolLoop) {
      printf("%% ");
      getline(&buff, &len, stdin);
      
      size_t inputLen = strlen(buff);
      if(inputLen == -1) {
         perror("Error reading input");
         break;
      }

      if(inputLen <= 1 || (strspn(buff, " \t\n\r\f\v") == inputLen && isspace(buff[0]))) {
         printf("Unrecognised command\n");
         continue;
      }

      if(inputLen > 0 && buff[inputLen - 1] == '\n') 
         buff[inputLen - 1] = '\0';

      command = parseCommand(buff);

      if(command == 0) {
         boolLoop = false;
         printf("ACK\n");
      }
      else if(command == 1 || command == 2) {
         id = parseId(buff);
         qty = parseQty(buff);
         if(id == NULL || qty == 65535) {
            printf("NAK Incorrect syntax.\n");
            continue;
         }
         if(command == 1)
            addItems(inventory, id, qty);
         else
            removeItems(inventory, id, qty);
         free(id);
         id = NULL;
      }
      else if(command == 3)
         printItems(inventory);
      else
         continue;
   }

   free(buff);
   buff = NULL;
   free(id);
   id = NULL;
}

int parseCommand(char* args) {
   char *command = strtok(args, " \t\0");
   if(command != NULL) {
      for (size_t i = 0; command[i]; ++i) {
         command[i] = tolower(command[i]);
      }
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
      printf("Unrecognised command\n");
      return -1;
   }
}

char* parseId(char* args) {
   char* parsedId = strtok(NULL, ":");
   if(parsedId == NULL || parsedId[0] == '\0') {
      return NULL;
   }

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
   char* parsedQty = strtok(NULL, " \t\0");
   if(parsedQty == NULL || parsedQty[0] == '\0') {
      return 65535;
   }

   for(size_t i = 0; parsedQty[i] != '\0' && parsedQty[i] != ' '; ++i) {
      if(parsedQty[i] < '0' || parsedQty[i] > '9') {
         return 65535;
      }
   }

   unsigned int qty;
   if (sscanf(parsedQty, "%u", &qty) != 1 || qty > USHRT_MAX) {
      return 65535;
   }

   return qty;
}

void addItems(itemNode* inventory, char* id, unsigned short qty) {
   while(inventory != NULL) {
      if(strcmp(inventory -> id, id) == 0) {
         inventory -> qty += qty;
         printf("ACK\n");
         return
            ;
      }
      inventory = inventory -> next;
   }
   printf("NAK Unknown Item\n");
}

void removeItems(itemNode* inventory, char* id, unsigned short qty) {
   while(inventory != NULL) {
      if(strcmp(inventory -> id, id) == 0) {
         int intQty = inventory -> qty;
         if((intQty -= qty) >= 0) {
            inventory -> qty -= qty;
            printf("ACK\n");
         } else {
            printf("NAK Insufficient Quantity\n");
         }
         return
            ;
      }
      inventory = inventory -> next;
   }
   printf("NAK Unknown Item\n");
}

void printItems(const itemNode *inventory) {
   printf("  %-32s %-8s %-15s\n", "DESC", "QTY", "ID");
   printf("-------------------------------  -------  ----------------\n");

   while (inventory != NULL) {
      printf("%-30s %8hu    %-15s\n", inventory -> desc, inventory -> qty, inventory -> id);
      inventory = inventory -> next;
   }
   printf("ACK\n");
}

void flushInventory(itemNode* inventory) {
   FILE* file = fopen("inv.dat.tmp", "w");
   if(file == NULL) {
      perror("Error creating temp file for writing");
      return
         ;
   }

   while(inventory != NULL) {
      fprintf(file, "%s:%hu:%s\n", inventory -> id, inventory -> qty, inventory -> desc);
      inventory = inventory -> next;
   }

   fclose(file);
   if(rename("inv.dat.tmp", "inv.dat") != 0) {
      perror("Error renaming temorary file");
   }
}

void freeInventory( itemNode* inventory) {
   itemNode* next;

   while(inventory != NULL) {
      next = inventory -> next;
      free(inventory);
      inventory = NULL;
      inventory = next;
   }
}

