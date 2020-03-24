#include <stdio.h>
#include <stdlib.h>
#include <string.h> 


void cutWithSpace(char * input, char* command, char **parameters)
{

char* token = NULL ;

token = strtok_r(input, 32);



if(token == NULL)
{
        return ;
}


command = token ;

int i = 0 ;

while(token != NULL && i <1000)
{
    token  = strtok_r(NULL, ' ');
    if(token != NULL )
    {
        parameters[i] = token ; 
        i++;
    }
    
}

return ; 

}


int main()
{

char * cPath = getenv("PATH");


char command[1000];

char *parameters[1000];

char * input ;

while(1)
{
 printf("#");
cutWithSpace(gets(),command, parameters );


}


return 0 ;
}