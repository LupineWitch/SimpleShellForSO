#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <signal.h>
#include <unistd.h>
#include <wait.h>
#include <fcntl.h>


void writeToHistoryFile(char* input)
{
    char* cUsr = getenv("USER");
    char* path;
    if( 0 > asprintf(&path, "/home/%s/.shHist", cUsr)) // Getting current users command history
    {
        printf("Failed to read path!");
        return;
    }
    FILE* histFile = fopen(path, "a"); // Creating file if it doesn't exist
    if(histFile == NULL)
    {
        printf("Failed to open history!");
        return;
    }
    fprintf(histFile,"%s\n", input); // Put input line in the front of the historyFile
    

    fclose(histFile); // Closing historyFile
    free(path); // Freeing path after allocing memory by asprintf()
}

void historyHandler()
{
    char* cUsr = getenv("USER");
    char* path;
    if( 0 > asprintf(&path, "/home/%s/.shHist", cUsr)) // Getting current users command history
    {
        printf("Failed to read path!");
        return;
    }
    FILE* histFile = fopen(path, "a+"); // Opening historyFile
    if(histFile == NULL)
    {
        printf("Failed to open history!");
        return;
    }

    int count = 0; // Count of lines in a file
    char c; // Temporary char storage
    for (c = getc(histFile); c != EOF; c = getc(histFile)) 
        if (c == '\n') // Increment count if this character is newline 
            count = count + 1;
    
    rewind(histFile); // Rewind historyFile to beggining
    char chunk[10000]; // Temporary buffer for reading file
    int currLine = 0; // Temporary current line storage
    printf("\nLast 20 commands history:\n");
    while(fgets(chunk, sizeof(chunk), histFile) != NULL)
    {
        if((count-currLine)<20) // Print only the 20 last lines
        {
            fputs(chunk, stdout); // Printing lines from history file
        }
        currLine++;
    }

    fclose(histFile); // Closing historyFile
    free(path); // Freeing path after allocing memory by asprintf()
}

int CheckForNativeCommands(char** parsedInput) //self explanatory 
{
    char* InternalCommands[2];
    InternalCommands[0] = "cd" ;
    InternalCommands[1] = "exit";
  
        if(strcmp(InternalCommands[0],parsedInput[0]) == 0)
        {
            if(parsedInput[1] != NULL)
            {
            chdir(parsedInput[1]);
            }
            else
            {
                printf("user failed to input proper diretory");
            }   
            return 1;       
        }
        else if(strcmp(InternalCommands[1],parsedInput[0]) == 0)
        {
            exit(0);
        }

        return 0;   
}


void forkAndExecute(char* args[],int flag,char *filename)
{
    int outFileDescriptor = 0;
     
    if(CheckForNativeCommands(args) != 0)
    {
        return;
    }

    if(((flag >> 3) & 1) == 1)
    {
            
            if(((flag >> 4) & 1) == 1)
            {
                outFileDescriptor = open(filename, O_WRONLY | O_APPEND | O_CREAT, S_IRWXG | S_IRWXO | S_IRWXU );
            }
            else
            {
                outFileDescriptor = open(filename, O_WRONLY | O_TRUNC | O_CREAT, S_IRWXG | S_IRWXO | S_IRWXU );
            }
            
    }
    if(outFileDescriptor < 0)
    {
        printf("Failed to create file %s",filename);
        return;
    }

    int ProcessID = fork(); //forking existing process

    if(ProcessID == -1 ) // if fail
    {
        printf("Failed to fork");
    }
    else if(ProcessID == 0)
    {
        if(((flag >> 3)& 1UL) == 1)
        {
            dup2(outFileDescriptor,1);
            close(outFileDescriptor);
        }
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
        if(((flag >> 3)& 1UL) == 1)
        {
            close(outFileDescriptor);
        }
        if(((flag >> 1)& 1UL ) == 0 )
        {
        waitpid(ProcessID,NULL,0); // wait for child to finish
        }
        else
        {
            return;
        }
    }
}

int cutWithSpace(char * input, char **parameters, unsigned long int* flag, char** parametersPiped, char* filename) 
{
    if(input == NULL) // If input is empty return error value -1
    {
        return -1;
    }
    writeToHistoryFile(input); // log cinput

    char* prevSeq = NULL; //setting helper pointer
    char* token = NULL; // Setting up our word buffer
    token = strtok_r(input, " ", &prevSeq); // Getting first word from input

    parameters[0] = token ; // First arg is a program name

    int i = 1 ; // Setting up loop variable
    while(token != NULL && i <1000) // While there is word and there aren't more than 1000 of them
    {
        token  = strtok_r(NULL, " ", &prevSeq); // Read the next word
        if(token != NULL ) // If it exists
        {
            switch (token[0]) // flag setting for pipes and background run
            {
                case 38: // AMPERSAND
                *flag |= 1UL << 1;
                break;
                case 124: // pipe
                *flag |= 1UL << 2;
                break;
                case 62: // triangle braces
                *flag |= 1UL << 3;
                if(token[1]!= '\0' && token[1]!= 0 && token[1]== 62)
                {
                    *flag |= 1UL << 4;
                }
                break;
                default:
                parameters[i]=token;
                break;
            }
            i++; // Keep track of loop iteration
        }
                if(*flag > 0) // & should be on the end of imput therefore stop parsing || if pipeflag set should parse another command
            {
                break;
            }
    }
    if(((*flag >> 3) & 1UL == 1))
    {
        token = strtok_r(NULL, " ", &prevSeq);
        strcpy(filename,token);
    }
    // same as above loop but for another pipe command
    int j = 0;
    while(token != NULL && ((*flag >> 2)& 1UL ) == 1  && j<1000)
    {
         token  = strtok_r(NULL, " ", &prevSeq); // Read the next word etc
         if(token != NULL)
         {
             parametersPiped[j] = token;
            j++;
         }
         
    }
    return 0; // All good
}


void PipeExecute(char** parameters, char** parameters2)
{
    // A fileDescriptor array
    int fileDescriptors[2];
    int PID1, PID2; // Process IDs

    if(pipe(fileDescriptors) < 0)
    {
        printf("\nFailed when creating PIPE");
    }

    PID1 = fork(); //forking frst child for first command

    if(PID1 < 0)
    {
        printf("\nFailed First Fork");
    }
    if(PID1 == 0) // if PID == 0 we are in child
    {
        //close reading input, beacuse we want to write to stdout
        close(fileDescriptors[0]);
        dup2(fileDescriptors[1],STDERR_FILENO); // Duplicating file descriptor to stdout
        close(fileDescriptors[1]); //close file descriptor for writing
       
        if(execvp(parameters[0],parameters)<0) // RUN command
        {
                      printf("Could not execute command: %s ", parameters[0]);
          int i = 1;
          while(puts(parameters[i])>=0)
          {
              putchar(32);
              i++;
          }
          exit(0); // Killing child 
        }
    }
    else //we are in parent
    {
        waitpid(PID1,NULL,0);

        PID2 = fork();
        if(PID2 < 0)
        {
            printf("Failed second fork");
        
        }
        if(PID2==0) //we are in second child
        {
            //close filedescriptor for output as we want to read from stdin
            close(fileDescriptors[1]);
            dup2(fileDescriptors[0],STDERR_FILENO); // duplicating file descriptor to stdin
            close(fileDescriptors[0]); //closing file descriptor
            if(execvp(parameters2[0],parameters2)<0) //try to run command
            {
             printf("Could not execute command: %s ", parameters[0]);
             int i = 1;
             while(puts(parameters[i])>=0)
                {
                    putchar(32);
                    i++;
                }
            exit(0); // Killing child 
            }
        }
        else
        {
            //wait for children to die
            waitpid(PID2,NULL,0);
        }
    }
}


int main()
{

    /*Flag Bits control
    1 - background running
    2 - pipes
    3 - Should redirect output?
    4 - 0/1 trunc/append mode
    
    */
    signal(SIGINT, historyHandler); // Changing SIGINT signal behaviour to showing history of commands

    char* cDir; // Setting up variable for current directory path
    char* cUsr; // Setting up variable for current user
    char *p; // helper pointer for finding newline char in input
   // char command[1000]; // Command read from the user input  - obsolete
    char* parameters[1000]; // Arguments of our command read from the user input
    char* parametersPiped[1000]; // Arguments of piped command read from the previous command output.
    char* filename = (char*)malloc(10000*sizeof(char));
    char* input = (char*)malloc(10000*sizeof(char)); // Setting up variable for users input
    unsigned long int flag = 0; // BIT control flag

    while(1)
    {
        flag = 0;
        memset(parameters, 0, 1000); // Clearing parameters
        memset(parametersPiped, 0, 1000); // Clearing parameters
        memset(input, 0, 10000); // Clearing input
        p = NULL; //clearing helper pointer
    // Getting current directory

       cDir = get_current_dir_name();

        cUsr = getenv("USER"); // Getting current user
        printf("%s : %s >> ", cUsr, cDir); // Displaying command prompt

        fgets(input, 10000, stdin); // Getting users input
        if( ( p=strchr( input,'\n') ) !=NULL ) //removing newline, thus cutting string after it
             {
                    *p= '\0';
             }
        // Splitting users input by spaces on [command] [parameters]
        if( cutWithSpace(input, parameters,&flag,parametersPiped,filename) == -1){ // If reading input was invalid
            printf("You provided wrong input!\n"); // Display error msg
            continue; // Skip this loop iteration
        } 
        else if( ((flag >> 2)& 1UL ) == 0)
        {
            forkAndExecute(parameters,flag,filename);
        }
        else
        {
            PipeExecute(parameters,parametersPiped);
        }
    }

    free(input);
    signal(SIGINT, SIG_DFL); // Changing behaviour of SIGINT signal to its default state
    return 0 ;
}