#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

//make directory
void mkdir(char pathName[]){
    // handle an empty path
    if (strcmp(pathName, "/") == 0 || strcmp(pathName) == 0){
        printf("MKDIR ERROR: no path provided \n");
        return;
    }
    char baseName[64];
    char dirName [128];

    struct NODE* parent = splitPath(pathName, baseName, dirName);
    //if the parent node is empty end the function
    //return error, already handled by split path function
    if(parent == NULL) return;

    struct NODE* child = parent->childPtr;
    while (child != NULL){
        if (child->fileType == 'D' && strcmp(child->name, baseName) == 0) {
            printf("MKDIR ERROR: directory %s already exists \n", pathName);
            return;
        }
        child = child->siblingPtr;
    }

    //creating new directory node
    struct NODE* newDir = (struct NODE*)malloc(sizeof(struct NODE));
    if (newDir == NULL) {
    printf("MKDIR ERROR: Memory allocation failed\n");
    return;
    }
    
    strncpy(newDir->name, baseName, 63);
    newDir->name[63] = '\0';
    newDir->fileType = 'D';
    newDir->childPtr = NULL;
    newDir->siblingPtr = NULL;
    newDir->parentPtr = parent;

    
    
    //add as a child to the parent node and add relations to sibling node
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
    //handles empty path
    if(strcmp(pathName, "/") == 0) {
        strcpy(dirName, ""); 
        strcpy(baseName, "");
        //returns to the current working directory since there is no other information
        return cwd;
    }
    
    //handles root case
    if(strcmp(pathName, "/") == 0) {
        strcpy(dirName, "/"); 
        strcpy(baseName, "");
        //returns to the root since that is the only thing in the path
        return root;
    }

    //copy pathName to avoind modifying the original string
    char tempPath[128];
    strcpy(tempPath, pathName);
    tempPath[127] = '\0';

    //traverse directory path 
    struct NODE* current = (pathName[0] == '/') ? root : cwd;
    char *start = (pathName[0] == '/') ? tempPath + 1: tempPath;

    //split path and seperate directory and base name 
    char *lastSlash = strrchr(start, '/');
    
    if (lastSlash != NULL) {
        //slashes in path, separate into dirname and basename
        *lastSlash = '\0'; //temp terminates dirname
        if(pathName[0] == '/') {
            strcpy(dirName, "/");
            strcat(dirName, start);
        } else {
            strcpy(baseName, lastSlash +1);
        }
    } else {
        //no slashes in the path
        strcpy(baseName, start);
        strcpy(dirName, "");
    }

    //if theres no file in the current directory return current
    if(strlen(dirName) == 0){
        return current;
    }

    char dirCopy[128]; 
    strcpy(dirCopy, dirName);
    char* token = strtok(dirCopy, "/");
    if(pathName[0] == '/') {
        token = strtok(NULL, "/"); //skips empty token for absolute paths
    }
    while (token != NULL) {
        struct NODE* next = NULL;
        struct NODE* child = current->childPtr;

        while (child != NULL){
            if(child->fileType == 'D' && strcmp(child->name, token) == 0 ){
                next = child;
                break;
            }
            child = child->siblingPtr;
        }
        /*
        this didnt want to work for me, so switched to while loop
        for(struct NODE* child = current->childPtr; child != NULL; child = child->siblingPtr) {
            if(strcmp(child->name, token) == 0 && child->fileType == 'D') {
                next = child;
                break;
            }
        }
        */
        //if directory is not found, return an error
        if(next == NULL) {
            printf("ERROR: directory %s does not exist \n", token);
            return NULL;
        }
        current = next;
        token = strtok(NULL, "/");
    }
    return current;
}
