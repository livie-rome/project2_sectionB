

#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

//make directory
void mkdir(char pathName[]){
    // handle an empty path
    if (strcmp(pathName, "/") == 0){
        printf("MKDIR ERROR: no path provided \n");
        return;
    }
    char baseName[64];
    char dirName [128];

    struct NODE* parent = splitPath(pathName, baseName, dirName);
    if(parent == NULL){
        return; //error handled by split path, just exists the function
    }

    struct NODE* child = parent->childPtr;
    while (child != NULL){
        if (child->fileType == 'D' && strcmp(child->name, baseName) == 0) {
            printf("MKDIR ERROR: director %s already exists /n", pathName);
            return;
        }
        child = child->siblingPtr;
    }

    struct NODE* newDir = (struct NODE*)malloc(sizeof(struct NODE));
    strncpy(newDir->name, baseName, 63);
    newDir->name[63] = '0\';
    newDir->fileType = 'D';
    newDir->childPtr = NULL;
    newDir->siblingPtr = NULL;
    newDir->parentPtr = parent;

    if(parent->childPtr == NULL){
        parent->childPtr = newDir;
    } else{
        child = parent->childPtr;
        while (child->siblingPtr != NULL) {
            child = child->siblingPtr;
        }
        child->siblingPtr = newDir;
    }
    

    printf("MKDIR SUCCESS: node %s successfully created \n", pathName);
}

//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName){

    // TO BE IMPLEMENTED
    // NOTE THAT WITHOUT COMPLETING THIS FUNCTION CORRECTLY
    // rm, rmdir, ls, cd, touch COMMANDS WILL NOT EXECUTE CORRECTLY
    // SEE THE PROVIDED EXECUTABLE TO SEE THEIR EXPECTED BEHAVIOR

    // YOUR CODE HERE

    //handle empty path
    if(strcmp(pathName, "/") == 0) {
        strcpy(dirName, "/"); 
        strcpy(baseName, "");
        return root;
    }

    //copy pathName to avoind modifying the original string
    char tempPath[128];
    strcpy(tempPath, pathName);

    //split path and seperate directory and base name 
    char *lastSlash = strrchr(tempPath, '/');
    if (lastSlash != NULL) {
        *lastSlash = '\0';
        strcpy(baseName, lastSlash + 1);
        strcpy(dirName, tempPath);
    } else {
        strcpy(baseName, pathName);
        strcpy(dirName, "");
    }

    //traverse directory path 
    struct NODE* current = (dirName[0]=='/') ? root : cwd;
    char *token = strtok(dirName, "/");

    while (token != NULL) {
        struct NODE* found = NULL;
        for(struct NODE* child = current->childPtr; child != NULL; child = child->siblingPtr) {
            if(strcmp(child->name, token) == 0 && child->fileType == 'D') {
                found = child;
                break;
            }
        }
        if(!found) {
            printf("ERROR: directory %s does not exist \n", token);
            return NULL;
        }
        current = found;
        token = strtok(NULL, "/");
    }
    return current;
}
