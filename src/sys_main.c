#include <graphics_lib.h>
#include <stdio.h>
#include <conio.h>
#include "sys_prog.h"

int main(void)
{

    /* Array to hold stack entries */
    int stack[10];
    int done = 0;
    int num_clicks = 0;

    /* Stack pointer */
    int *stack_p = stack;

    initwindow(200, 200);
    setbkcolor(BLACK);
    setcolor(BLUE);
    initmouse();

    /* create an event queue */
    create_event_queue();

    /* register display and mouse as event sources */
    reg_display_events();
    reg_mouse_events();



    /* Finish this! */
    while (!done)
    {
        /* wait for event  */
        wait_for_event();
        if (event_mouse_button_down())
        {
            if (event_mouse_left_button_down()) /* if left mouse button down */
            {
//                done = 1;
                num_clicks++;
                printf("position == %d \r\n",stack_p);
                printf("addr of stack_p == %d \r\n",&stack_p);
                push(&stack_p,5);
                //printf("position == %d \r\n",&stack_p);
//                printf("Popped value: %d \r\n", pop(&stack_p));
            }

        }
        if(num_clicks==10)
        {
            done=1;
            num_clicks=0;
        }

    }
    /* close the mouse */
    closemouse();

    return 0;

}
