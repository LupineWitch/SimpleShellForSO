#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h>
#include <wait.h>




void forkAndExecute(char* args[])
{

    int ProcessID = fork(); //forking existing process

    if(ProcessID == -1 ) // if fail
    {
        printf("Failed to fork");
    }
    else if(ProcessID == 0)
    {
         /* The execvp uses built array of arguments, the first argument being filename of program being run */
      if( execvp(args[0],args) < 0 ) // chec if exec  failed  
      {
          printf("Could not execute command: %s ", args[0]);
          int i = 1;
          while(puts(args[i])>=0)
          {
              putchar(32);
              i++;

          }
          exit(0); // Killing child 
       
      }
    }
    else
    {
        waitpid(ProcessID,NULL,0); // wait for child to finish
    }
    


}

int cutWithSpace(char * input, char **parameters)
{
    if(input == NULL) // If input is empty return error value -1
    {
        return -1;
    }

    char* prevSeq = NULL;
    char* token = NULL; // Setting up our word buffer
    token = strtok_r(input, " ", &prevSeq); // Getting first word from input

    parameters[0] = token ; // First arg is a program name

    int i = 1 ; // Setting up loop variable
    while(token != NULL && i <1000) // While there is word and there aren't more than 1000 of them
    {
        token  = strtok_r(NULL, " ", &prevSeq); // Read the next word
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
    char* cDir; // Setting up variable for current directory path
    char* cUsr; // Setting up variable for current user
    char *p; // helper pointer for finding newline char in input
   // char command[1000]; // Command read from the user input  - obsolete
    char* parameters[1000]; // Arguments of our command read from the user input
    char* input = (char*)malloc(10000*sizeof(char)); // Setting up variable for users input

    int readFlag; // Setting up flag for correct splicing and reading operation

    while(1)
    {
        memset(parameters, 0, 1000); // Clearing parameters
        memset(input, 0, 10000); // Clearing input
        p = NULL; //clearing helper pointer
        readFlag = 0;

        cDir = getenv("PWD"); // Getting current directory
        cUsr = getenv("USER"); // Getting current user
        printf("%s : %s >> ", cUsr, cDir); // Displaying command prompt

        fgets(input, 10000, stdin); // Getting users input
        if( ( p=strchr( input,'\n') ) !=NULL ) //removing newline, thus cutting string after it
             {
                    *p= '\0';
             }
        readFlag = cutWithSpace(input, parameters); // Splitting users input by spaces on [command] [parameters]
        if(readFlag == -1){ // If reading input was invalid
            printf("You provided wrong input!\n"); // Display error msg
            continue; // Skip this loop iteration
        } 
        else
        {
            forkAndExecute(parameters);
        }
    }

    return 0 ;
}