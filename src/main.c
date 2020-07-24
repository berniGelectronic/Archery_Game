/*
-------------------------------------------------------------------------------------------
***********************************Archery Game by Berni***********************************
-------------------------------------------------------------------------------------------

##This is archery game, assignment project for C programming(ELE00107M) course
  @University of York, Department of Electronic Engineering
##@Student Bernard
##@Date January 2020

##Sound libraries are from Dimitri Zantalis - clab/audio07.c example
##audio_lib API
## File:   ex_fileplayback.c
## Author: Dimitri Zantalis
## Date:   September 2013
##Copyright (c) 2013 Dimitri Zantalis <dz548@york.ac.uk>

##Sound effects .wav are downloaded from https://www.sounds-resource.com/genre/racing/R.html
*/

#include <stdio.h>
#include <graphics_lib.h>
#include "archery.h"

const char *wind_direction[]= {'N','S','E','W','\0'};

int main()
{
    int done=0, click=0;
    int x_pos,y_pos,num_clicks;
    int last_x,last_y;
    int angle;
    int i,j;
    int score=0, level=0;
    int wind, wind_dir;
    int hit_miss=0,count_hit=0;
    int try_ag=0;
    int *hit_miss_p=&hit_miss;
    int *arrow_angle=&angle;
    int *x_p=&x_pos;
    int *y_p=&y_pos;

    initwindow(X_WINDOW,Y_WINDOW);
    initfont();
    initmouse();
    create_event_queue();
    reg_mouse_events();
    welcome_screen(x_p,y_p);

    for(j=1; j<=LEVEL_NUMBER; j++) //levels
    {
        if(j==1 || level!=0) //level 0 or without hit on current level
        {
            count_hit=0; // new level set hits to zero
            for(i=ARROW_NUMBER; i>0; i--) //arrows number
            {
                if(j<=5)
                {
                    wind=random_number(1,5);
                }
                else
                {
                    wind=random_number(5,10);
                }
                landscape();
                draw_forest(TREE_NUMBER);
                tents();
                target();
                draw_player();
                setcolor(RED);
                wind_dir=random_number(0,3);
                print_wind(wind,wind_direction[wind_dir]);
                print_arrow_left(i);
                print_score(score);
                setcolor(BLACK);
                print_level(j);
                update_display();
                if(i==ARROW_NUMBER && j==1) // after click on play button, draws bow on the position dependent on the coordinates of click
                {
                    setcolor(BLACK);
                    draw_bow(*y_p,x_p,y_p,arrow_angle);
                    update_display();
                    setcolor(GREEN);
                    draw_bow(*y_p,x_p,y_p,arrow_angle);
                }
                else // draws bow on the last shooting position
                {
                    setcolor(BLACK);
                    draw_bow(last_y,x_p,y_p,arrow_angle);
                    update_display();
                    setcolor(GREEN);
                    draw_bow(last_y,x_p,y_p,arrow_angle);
                }

                while (!done)
                {
                    wait_for_event();
                    if (event_mouse_position_changed() && click==0 )
                    {
                        get_mouse_coordinates();
                        setcolor(BLACK);
                        draw_bow(YMOUSE,x_p,y_p,arrow_angle);
                        update_display();
                        setcolor(GREEN);
                        draw_bow(YMOUSE,x_p,y_p,arrow_angle);
                        last_x=XMOUSE;
                        last_y=YMOUSE; // save last y when in this if, after 1st click, draw bow on the same position!
                    }
                    if (event_mouse_button_down())
                    {
                        if (event_mouse_left_button_down())
                        {
                            click++;
                            if(click==1)
                            {
                                main_audio(BOW_PREPARATION);
                            }
                            if(click==2)
                            {
                                main_audio(ARROW_FLY);
                            }
                        }
                    }
                    if(click==1) // after first click, draw strength bar dependent on the x position of mouse
                    {
                        setcolor(BLACK);
                        outtextxy(300,50,"--->>>STRENGTH--->>>");
                        get_mouse_coordinates();
                        last_x=XMOUSE;
                        draw_bow(last_y,x_p,y_p,arrow_angle);
                        draw_strength_bar(XMOUSE); // draw bar
                        update_display();
                        filled_rectangle(200,20,600,50,LIGHTBLUE); //cover strength bar, every move of the mouse draws new bar with new strength
                    }
                    else if(click==2)
                    {
                        draw_strength_bar(last_x);
                        score=score + (0.5*i)*score_value(draw_arrow_path(*x_p,*y_p,scale_number(last_x,0,X_WINDOW,0,90),
                                abs(*arrow_angle),
                                wind,wind_direction[wind_dir],hit_miss_p));
                        //score takes into account 50% of the current arrow out of total number, first arrow hit give more points
                        update_display();

                        if(hit_miss!=0)
                        {
                            main_audio(CROWD);
                            count_hit++; // count hits
                            level++; // increase level variable
                        }
                        else
                        {
                            main_audio(MISS_TARGET);
                        }
                        if(count_hit==0)//if there is no hit at any leave level -> end of game
                        {
                            level=0;
                        }
                        click=0;
                        done=1;
                    }
                }
                if(done==1)
                {
                    done=0;
                }
                if(i==1 && count_hit==0 || j==10) // last arrow and zero hits or last level
                {
                    try_ag=end_screen(x_p,y_p,score,j);
                }
                if(try_ag==1)  // try again
                {
                    try_ag=0;
                    i=ARROW_NUMBER+1;  //
                    score=0;
                    j=1;
                    pausefor(100);
                }
                else if(try_ag==2)
                {
                    pausefor(100);
                    closegraph();
                }
            }

        }

    }
    getch();
    closemouse();
    closegraph();
    return 0;
}




