#include "sys_prog.h"

void push(int **sp, int value)
{
    **sp=value;
    *sp=*sp+1;


    printf(" sp val :: %d \n",sp);
    printf(" sp one ptr val:: %d \n",*sp);
    printf(" sp_double val:: %d \n",**sp);

    /* Finish this! */
}

int pop(int **sp)
{
    int var=0;
//	var=**sp;
    //*(sp--);
    return var;
}
