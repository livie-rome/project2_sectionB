#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

//make directory
void mkdir(char pathName[]){
    // handle an empty path
    if (strcmp(pathName, "/") == 0 || strcmp(pathName, "") == 0){
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
        //throw error if the directory already exists
        if (child->fileType == 'D' && strcmp(child->name, baseName) == 0) {
            printf("MKDIR ERROR: directory %s already exists \n", pathName);
            return;
        }
        //move onto a sibling
        child = child->siblingPtr;
    }

    //creating new directory node
    struct NODE* newDir = (struct NODE*)malloc(sizeof(struct NODE));
    
    //throws error if newDir isn't properly allocated
    if (newDir == NULL) {
        printf("MKDIR ERROR: Memory allocation failed\n");
        return;
    }
    
    //initiating values in newDir
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
    if(strlen(pathName) == 0) {
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

    //copy pathName to avoid modifying the original string
    char tempPath[128];
    strcpy(tempPath, pathName);
    tempPath[127] = '\0';

    //traverse directory path 
    struct NODE* current = (pathName[0] == '/') ? root : cwd;
    char* pathStart = (pathName[0] == '/') ? tempPath + 1 : tempPath;

    //split path and seperate directory and base name 
    char *lastSlash = strrchr(pathStart, '/');
    
    if (lastSlash != NULL) {
        //slashes in path, separate into dirname and basename
        *lastSlash = '\0'; 
        if(pathName[0] == '/') {
            strcpy(dirName, "/");
            strcat(dirName, pathStart);
        } else {
            strcpy(dirName, pathStart);
        }
        strcpy(baseName, lastSlash + 1);
    } else {
        //no slashes in the path
        strcpy(dirName, "");
        strcpy(baseName, pathStart);
    }

    //if theres no file in the current directory return current
    if(strlen(dirName) == 0){
        return current;
    }

    char dirCopy[128]; 
    strcpy(dirCopy, dirName);
    char* token = strtok(dirCopy, "/");
    
    if(pathName[0] == '/') {
        //skips empty token for absolute paths
        token = strtok(NULL, "/"); 
    }

    //iterate through each token in path
    while (token != NULL) {
        struct NODE* next = NULL;
        struct NODE* child = current->childPtr;
        
        //traverse the linked list of children
        while (child != NULL){
            if(child->fileType == 'D' && strcmp(child->name, token) == 0 ){
                //if theres a match, set the node next as child
                next = child; 
                //break the while loop
                break; 
            }
            child = child->siblingPtr; //move to next symbol
        }
        
        /*
        this didnt want to work for me, so switched to while loop
        struct NODE* next = NULL;
        for(struct NODE* child = current->childPtr; child != NULL; child = child->siblingPtr) {
            if(strcmp(child->name, token) == 0 && child->fileType == 'D') {
                next = child;
                break;
            }
        }
        */
        
        //if no mathcing directory is found, return an error
        if(next == NULL) {
            printf("ERROR: directory %s does not exist \n", token);
            return NULL;
        }
        //set current to the found directory node
        current = next;
        //get the next token from the path
        token = strtok(NULL, "/");
    }
    //returns the path to be used by mkDir
    return current;
}
