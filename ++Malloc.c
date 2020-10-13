#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>



//main driver function
int main()
{	
	//pointer for input
    char* test;

    int limit= 30;
     
    //allocate memory dynamically
    test = (char*)malloc(limit*sizeof(test));
    
    //prompt and store input 
    printf("Enter string: ");
    gets(test);
 
    //free allocated memory
    free(test); 
    return 0;
}

// create error handlers with main above

