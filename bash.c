#include <stdio.h>
#include <stdlib.h>
#include <string.h> 


int cutWithSpace(char * input, char* command, char **parameters)
{
if(input == NULL) // If input is empty return error value -1
{
    return -1;
}

char* token = NULL ; // Setting up our word buffer
token = strtok(input, " "); // Getting first word from input

command = token ; // The first word in input is a command

int i = 0 ; // Setting up loop variable
while(token != NULL && i <1000) // While there is word and there aren't more than 1000 of them
{
    token  = strtok(NULL, " "); // Read the next word
    if(token != NULL ) // If it exists
    {
        parameters[i] = token ;  // Put it as next argument on our list
        i++; // Keep track of loop iteration
    }
}

return 0; // All good
}


int main()
{
char * cPath = getenv("PATH"); // Environment variable $PATH
char * cDir; // Setting up variable for current directory path
char * cUsr; // Setting up variable for current user

char command[1000]; // Command read from the user input
char *parameters[1000]; // Arguments of our command read from the user input
char *input = (char*)malloc(10000*sizeof(char)); // Setting up variable for users input

int readFlag; // Setting up flag for correct splicing and reading operation

while(1)
{
    cDir = getenv("PWD"); // Getting current directory
    cUsr = getenv("USER"); // Getting current user

    printf("%s : %s >> ", cUsr, cDir); // Displaying command prompt

    fgets(input, 10000, stdin); // Getting users input
    readFlag = cutWithSpace(input, command, parameters); // Splitting users input by spaces on [command] [parameters]
    if(readFlag == -1){ // If reading input was bad
        printf("You provided wrong input!\n"); // Display error msg
        continue; // Skip this loop iteration
    } 

    //Testing block #1
    /*
        TODO: 
        CORRECT READING FROM LINE
        PURGING THE COMMAND AND PARAMETERS LIST BETWEEN COMMANDS
    */
    printf("The command is %s\n", command); 
    int i = 0;
    while(parameters[i] != NULL)
    {
        printf("Parameter %d was %s\n", i, parameters[i]);
        i++;
    }
}


return 0 ;
}