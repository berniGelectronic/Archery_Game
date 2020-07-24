
#include <graphics_lib.h>
#include <amio_lib.h>
#include <conio.h>
#include "archery.h"
#include <math.h>

//#define DEBUG  // debug option allows to print all debug info, comment to turn off

typedef struct
{
    SIGNAL drums;
    long int ridx;
    float volume;
    float pan;
    char message[20];
}
USERDATA;

/** Function draws welcome screen with short introduction to game and play button
 *
 * *x_pos - set last X position of mouse
 * *y_pos - set last Y position of mouse
 *
 */
void welcome_screen(int *x_p, int *y_p)
{
    int x_w=100,y_w=100;
    int off=150;
    int play_x_st=250,play_y_st=400;
    int play_x_end=550,play_y_end=450;
    int done=0;
    int x_pos,y_pos;
    landscape();
    filled_rectangle(x_w,y_w,X_WINDOW-x_w,Y_WINDOW-y_w,LIGHTCYAN);
    initfont();
    setcolor(BLACK);
    outtextxy(x_w+off,100,"WELCOME TO ARCHERY GAME by Berni");
    outtextxy(x_w,140,"HOW TO PLAY::");
    outtextxy(x_w,160," MOUSE Up And Down selects the ANGLE of ARROW!");
    outtextxy(x_w,180," LEFT Click!");
    outtextxy(x_w,200," MOUSE Left And Right Selects the Strength of SHOT!");
    outtextxy(x_w,220," Left Click To Shot Arrow!");
    outtextxy(x_w,240," !!!Be Aware of The WIND!!!");
    outtextxy(x_w,300,"RULES::");
    outtextxy(x_w,320," ->You Have 3 Arrows Per Level");
    outtextxy(x_w,340," ->One Hit Leads You To The Next Level");
    outtextxy(x_w+135,380," When You Are Ready Click PLAY BUTTON!!");
    filled_rectangle(play_x_st,play_y_st,play_x_end,play_y_end,YELLOW); //play button
    setcolor(RED);
    outtextxy(375,420," PLAY ");

    update_display();
    main_audio(INTRO);
    while (!done)
    {
        wait_for_event();
        get_mouse_coordinates();
        x_pos = XMOUSE;
        y_pos = YMOUSE;
        if (event_mouse_left_button_down()) /* if left mouse button down */
        {
            //checks whether mouse is in within button coordinates
            if(x_pos>=play_x_st && x_pos<= play_x_end && y_pos>play_y_st && y_pos <=play_y_end)
            {
                done = 1;
                main_audio(CLICK);
                *x_p=x_pos;
                *y_p=y_pos;
#ifdef DEBUG
                printf("\nBUTTON X_pos: %d",x_pos);
                printf("\nBUTTON CLICKED!!!");
                printf("\ndone= %d", done);

#endif // DEBUG
                break;
            }
        }
    }
}

/** Function landscape draws background in three sections
 *  based on percentage of X, Y size of the window, fully scalable
 *
 *
 */
void landscape()
{
    int i;
    //Background colors
    //Implemented with percentage of X,Y Window
    filled_rectangle( 0, 0, X_WINDOW,.2*Y_WINDOW,LIGHTBLUE);     // sky
    filled_rectangle( 0, .2*Y_WINDOW,X_WINDOW,.95*Y_WINDOW,GREEN);    // landscape
    filled_rectangle( 0, .95*Y_WINDOW, X_WINDOW,Y_WINDOW,BROWN);    // soil
    filled_circle(X_WINDOW,0,.1*X_WINDOW,YELLOW); //sun
    setcolor(BLACK);
    line(0,.95*Y_WINDOW,X_WINDOW,.95*Y_WINDOW,2); //bottom line
}

/** Function trees - draws tree on specific position
 *
 * x_pos - x position of the tree
 * y_pos - Y position of the tree
 * scale - size of the tree
 * thickness - of the tree
 *
 */
void trees(int x_pos, int y_pos, float scale,int thickness)
{
    int y_default=.95*Y_WINDOW;
    filled_rectangle(x_pos,y_pos,x_pos+scale*thickness,y_pos+(y_default-y_pos)*scale,BROWN);
    filled_circle(x_pos,y_pos,0.05*X_WINDOW*scale,DARKGRAY);
    filled_circle(x_pos+x_pos*0.05*scale,y_pos+y_pos*scale*0.02,0.05*X_WINDOW*scale,DARKGRAY);
    filled_circle(x_pos-x_pos*0.05*scale,y_pos+y_pos*scale*0.03,0.05*X_WINDOW*scale,DARKGRAY);
    filled_circle(x_pos+scale*thickness,y_pos+y_pos*scale*0.03,0.05*X_WINDOW*scale,DARKGRAY);
    filled_circle(x_pos+x_pos*0.1*scale,y_pos+y_pos*scale*0.01,0.05*X_WINDOW*scale,DARKGRAY);
    filled_circle(x_pos+x_pos*0.05*scale,y_pos-y_pos*scale*0.04,0.05*X_WINDOW*scale,DARKGRAY);
    filled_circle(x_pos+x_pos*0.01*scale,y_pos-y_pos*scale*0.1,0.05*X_WINDOW*scale,DARKGRAY);

}
/** Function generates number of trees on random positions
 *
 * trees_number - Number of trees to be drawn
 *
 *
 */
void draw_forest(int trees_number)
{
    int i;
    for(i=0; i<trees_number; i++)
    {
        trees(random_number(0,X_WINDOW),random_number(100,250),
              rand_nmbr(0,1),random_number(20,50));
    }
}
/** Function Draws 3 static tents
 *
 *
 */
void tents()
{
    int i;
    int tent_xy[3][6]=
    {
        {150,400,200,300,250,400},
        {350,350,400,250,450,350},
        {550,450,600,350,650,450}
    };
    setcolor(BLACK);
    for(i=0; i<3; i++)
    {
        filled_triangle(tent_xy[i][0],tent_xy[i][1],tent_xy[i][2],tent_xy[i][3],tent_xy[i][4],tent_xy[i][5],BLUE);
        line(tent_xy[i][0],tent_xy[i][1],tent_xy[i][2]+10,tent_xy[i][3]-20,2);
        line(tent_xy[i][4],tent_xy[i][5],tent_xy[i][2]-10,tent_xy[i][3]-20,2);
    }

}

/** Function Draws target with holder
 *
 *
 *
 */
void target()
{
    int target_size[5][2]= {{25,50},{20,40},{15,30},{10,20},{5,5}};
    int target_color[]= {15,0,1,4,14};
    int i;

    line(X_TARGET,Y_TARGET,X_TARGET,Y_TARGET+70,5); // holder
    line(X_TARGET,Y_TARGET-40,X_TARGET+70,Y_TARGET+70,5); // holder

    //drawing target from biggest to smallest
    for(i=0; i<5; i++)
    {
        filled_ellipse(X_TARGET,Y_TARGET,target_size[i][0],target_size[i][1],target_color[i]);
    }
}


/** Function draws bow on different angle dependent on the y coordinate of mouse
 *
 * y_position - Y position of mouse
 * *x_p - pointer to x position of mouse
 * *y_p - pointer to y position of mouse
 * *angle - pointer to angle of bow
 *
 *
 */
void draw_bow(int y_position, int *x_p, int *y_p, int *angle)
{
    int radius=80,start_angle, end_angle=90;
    int start_angle_max= -120, start_angle_min= -30;
    int thickness=2, min_scale=0;
    int scale_tan;
    float arrow_scale=1.3;
    int x_pos,y_pos;
    start_angle=scale_number(y_position,min_scale,Y_WINDOW,start_angle_max,start_angle_min);
//bow
    arc(X_BOW_CENTER,Y_BOW_CENTER,radius,start_angle,end_angle,thickness);
    line(X_BOW_CENTER+cos(rad2deg(start_angle))*radius,
         Y_BOW_CENTER+sin(rad2deg(start_angle))*radius,
         X_BOW_CENTER-cos(rad2deg(end_angle-start_angle))*radius,
         Y_BOW_CENTER+sin(rad2deg(end_angle-start_angle))*radius,thickness);
//arrow
    scale_tan=scale_number(start_angle,start_angle_min,start_angle_max,110,0);
    *angle=rad2deg(start_angle);
    *x_p=X_BOW_CENTER-cos(rad2deg(end_angle+scale_tan))*radius*arrow_scale;
    *y_p=Y_BOW_CENTER-sin(rad2deg(end_angle+scale_tan))*radius*arrow_scale;
    line(X_BOW_CENTER,
         Y_BOW_CENTER,
         *x_p,
         *y_p,thickness);

#ifdef DEBUG
    printf("\nSTART_ANGLE:: %d ",start_angle);
    printf("\nScale_TAN:: %d,  Y_end_ARROW:: %f ",scale_tan,
           Y_BOW_CENTER+tan(rad2deg((end_angle+scale_tan)))*radius );
    printf("\nCALCU X_start:: %f ",x_center+cos(rad2deg(start_angle))*radius);
#endif

}

/** draw_player function draws static player
 *
 *
 */
void draw_player()
{
    int x_startLleg=10, x_startRleg=50;
    int x_endLleg=30, x_endRleg=30;
    int y_startLleg=570, y_startRleg=570;
    int y_endLleg=540, y_endRleg=540;
    int x_startBody=30, x_endBody=30;
    int y_startBody=540, y_endBody=480;
    int x_startLarm=30, x_startRarm=30;
    int x_endLarm=50, x_endRarm=50;
    int y_startLarm=500, y_startRarm=500;
    int y_endLarm=500, y_endRarm=520;
    int x_head=30, y_head=480, head_dia=15;
    int width_person=2;

    setcolor(WHITE);
    line(x_startLleg,y_startLleg,x_endLleg,y_endLleg,width_person);   //left leg
    line(x_startRleg,y_startRleg,x_endRleg,y_endRleg,width_person);   //right leg
    line(x_startBody,y_startBody,x_endBody,y_endBody,width_person);   //Body
    line(x_startLarm,y_startLarm,x_endLarm,y_endLarm,width_person);   //left arm
    line(x_startRarm,y_startRarm,x_endRarm,y_endRarm,width_person);   //right arm
    filled_circle(x_head,y_head,head_dia, CYAN);
}

/** Function draws projectile path based on starting position of arrow
 *  strength, angle of bow and wind direction
 *
 * x_start_projectile - arrow tip starting x position
 * y_star_projectile - arrow tip starting y position
 * velocity - scaled strength
 * angle  - angle of bow
 * wind - random strength of wind
 * wind_dir - random wind direction *char*
 * hit_miss - set hit(1) or miss(0) value
 *
 * Returns:
 * i - number on which target projectile landed
 *
 */
int draw_arrow_path(int x_start_projectile, int y_start_projectile,
                         int velocity, int angle, int wind, char wind_dir, int *hit_miss)
{
    int x_endRarm=0,y_endRarm=y_start_projectile;
    int target_size[5][2]= {{25,50},{20,40},{15,30},{10,20},{5,5}};
    double pos_x, pos_y, init_pos_x, init_pos_y, vel_x, vel_y;
    double time, gravity;
    int x_vel_off=30,y_vel_off=20,constant=1;
    int x_vel,y_vel;
    int i,j;
    int done=0;

    if(wind_dir=='N')
    {
        y_vel=-(wind*constant);
    }
    else if(wind_dir=='S')
    {
        y_vel=wind*constant;
    }
    else if(wind_dir=='W')
    {
        x_vel=wind*constant;
    }
    else
    {
        x_vel=-(wind*constant);
    }

    setcolor(BLACK);
    vel_x = velocity*cos(angle*180/PI)+x_vel_off+x_vel;
    vel_y = velocity*sin(angle*180/PI)+y_vel_off+y_vel;
    gravity = 9.81;
    moveto(x_endRarm + x_start_projectile,y_endRarm);
    init_pos_x=x_endRarm + x_start_projectile;
    pos_x=x_endRarm + x_start_projectile;
    init_pos_y=y_endRarm;
    pos_y = y_endRarm;

    do
    {
        time = (pos_x - init_pos_x) / vel_x;
        pos_y = (int)(init_pos_y - (vel_y * time) + (gravity * time * time)/2);
        lineto(pos_x, pos_y, 3);
        pos_x++;
        if(pos_x==X_TARGET)
        {
            for(i=4; i>=0; i--)
            {
                //checks from the smallest circle to the biggest, if hit, break for
                if(pos_y>Y_TARGET-target_size[i][1] && pos_y < Y_TARGET+target_size[i][1])
                {
                    done=1;
                    break;
                }
            }
        }
        if(done==1)
        {
            break;//break do-while
        }
    }
    while (pos_y < 0.95*Y_WINDOW);
#ifdef DEBUG
    printf("\n Y projectile: %d \n", pos_y);
    printf("\n X projectile: %d \n", pos_x);
    printf("DONE:: %d",done);
#endif
    if(done==1)
    {
        print_hit_miss(done); // print text hit
        *hit_miss=1;
        return i;
    }
    else
    {
        print_hit_miss(done); // print text miss
        *hit_miss=0;
        return 5; //miss
    }
}

/** Functions draws strength bar on top section dependent on X position of mouse
 *
 * x_pos - X position of mouse
 *
 *
 */
void draw_strength_bar(int x_pos)
{
    int colors[]= {15,15,14,14,10,10,10,2,2,2,4,4,4};
    int x_rec=200,y_rec=25;
    int x_size=25, y_size=25;
    int thickness=2;
    int i=0;
    int x_scaled;

    x_scaled=scale_number(x_pos,0,X_WINDOW,0,sizeof(colors)/sizeof(int)); // scale 0 to 800 pixels to 0 to number of elements in color array
    for(i=0; i<=x_scaled; i++)
    {
        filled_rectangle(x_rec+(i*x_size),y_size,x_rec+x_size*(i+1),y_size+y_rec,colors[i]);
    }
#ifdef DEBUG
    printf("\n X_SCALED:: %d \n", x_scaled);
#endif // DEBUG

}

/** Function returns value of scored target
 *
 * score_pos - target on which arrow landed
 *
 * Return:
 * score value
 */
int score_value(int score_pos)
{
    int scores[]= {2,4,6,8,10,0};
#ifdef DEBUG
    printf("x_pos arrow :: %d \n", score_pos);
#endif // DEBUG

    return scores[score_pos];
}

/** Function draws end screen with scored value, level number, 2 buttons(try again, exit)
 *  play motivational sound and print quotes
 *
 * *x_pos - set last X position of mouse
 * *y_pos - set last Y position of mouse
 * score - score value to print
 * level - reached level
 *
 * Return
 * try_again = 1 --> play again,
 * try_again = 2 --> exit
 */
int end_screen(int *x_p, int *y_p, int score, int level)
{
    int x_w=100,y_w=100;
    int off=150;
    int play_x_st=250,play_y_st=300; //try again button
    int play_x_end=550,play_y_end=350;  //try again button
    int play_exx_st=250,play_exy_st=400; //exit button
    int play_exx_end=550,play_exy_end=450; //exit button
    int done=0;
    char label[50];
    int x_pos,y_pos;
    int try_again=0;
    landscape();
    filled_rectangle(x_w,y_w,X_WINDOW-x_w,Y_WINDOW-y_w,LIGHTCYAN);
    initfont();
    setcolor(BLACK);
    sprintf(label,"%d",level);
    outtextxy(x_w+off,100,"THANKS FOR PLAYING ARCHERY GAME by Berni!");
    outtextxy(x_w,200,"YOU REACHED LEVEL::");
    outtextxy(x_w+160,200,label);
    sprintf(label,"%d",score);
    outtextxy(x_w,220,"YOU SCORED::");
    outtextxy(x_w+100,220,label);
    outtextxy(x_w+140,220,"POINTS");
    switch(level) // prints different "motivational" message after each end
    {
    case 0:
        sprintf(label,"%s","->Bad Luck My Friend!<--");
        outtextxy(x_w,140,label);
        break;
    case 1:
    case 2:
    case 3:
    case 4:
        sprintf(label,"%s","-->Try Harder!<--");
        outtextxy(x_w,140,label);
        break;
    case 5:
    case 6:
    case 7:
        sprintf(label,"%s","-->Almost There, Don't Give Up!<--");
        outtextxy(x_w,140,label);
        break;
    case 8:
    case 9:
        sprintf(label,"%s","-->Soo Closee!<--");
        outtextxy(x_w,140,label);
        break;
    case 10:
        sprintf(label,"%s","-->!!!!!CONGRATULATIONS!!!!!<--");
        outtextxy(x_w,140,label);
        break;
    }
    filled_rectangle(play_x_st,play_y_st,play_x_end,play_y_end,YELLOW); //try again button
    filled_rectangle(play_exx_st,play_exy_st,play_exx_end,play_exy_end,YELLOW); //exit button
    setcolor(RED);
    outtextxy(360,320," TRY AGAIN ");
    outtextxy(375,420," EXIT ");
    update_display();
    if(level<=5)
    {
        main_audio(GOOD); // for levels 1 to 5 plays Good sound
    }
    else
    {
        main_audio(STAY_FOCUSED); // for levels 6 to 10 plays stay focused sound
    }

    while (!done)
    {
        wait_for_event();
        get_mouse_coordinates();
        x_pos = XMOUSE;
        y_pos = YMOUSE;
        if (event_mouse_left_button_down()) /* if left mouse button down */
        {
            //checks whether mouse is in within TRY AGAIN button coordinates
            if(x_pos>=play_x_st && x_pos<= play_x_end && y_pos>play_y_st && y_pos <=play_y_end)
            {
                main_audio(CLICK);
                done = 1;
                try_again=1;
                *x_p=x_pos;
                *y_p=y_pos;
                return try_again;
#ifdef DEBUG
                printf("\nBUTTON X_pos: %d",x_pos);
                printf("\nBUTTON CLICKED!!!");
                printf("\ndone= %d", done);
#endif // DEBUG
            }
            //checks whether mouse is in within EXIT button coordinates
            if(x_pos>=play_exx_st && x_pos<= play_exx_end && y_pos>play_exy_st && y_pos <=play_exy_end)
            {
                main_audio(CLICK);
                try_again=2;
                return try_again;
            }
        }
    }
}

/** Function print text MISS or HIT in the top right corner
 *
 * hit=1 --> HIT
 * hit=0 --> MISS
 *
 */
void print_hit_miss(int hit)
{
    if(!hit)
    {
        outtextxy(600,40,"!!MISS!!");
    }
    else
    {
        outtextxy(600,40,"!!HIT!!");
    }
}

/** Function prints scored value in top left corner
 *
 * score - score to print on the screen
 *
 */
void print_score(int score)
{
    char label[3];
    initfont();
    sprintf(label,"%d",score);
    outtextxy(10,40,"SCORE: ");
    outtextxy(80,40,label);
}

/** Function prints number of arrow left per level in top left corner
 *
 * arrow_count - number of arrow left per level
 *
 */
void print_arrow_left(int arrow_count)
{
    char label[3];
    sprintf(label,"%d",arrow_count);
    outtextxy(10,10,"ARROW LEFT: ");
    outtextxy(100,10,label);
}

/** Function prints strength and direction of wind in top right corner
 *
 * wind - strength of wind
 * wind_dir - First upper case character of wind direction
 *
 */
void print_wind(int wind, char wind_dir)
{
    char label[3];
    sprintf(label,"%d",wind);
    outtextxy(600,10,"WIND: ");
    outtextxy(650,10,label);
    sprintf(label,"%c",wind_dir);
    outtextxy(670,10,label);
#ifdef DEBUG
    printf("WIND DIR::: %c",wind_dir);
#endif // DEBUG
}

/** function prints level on the bottom middle screen
 *
 * level_number - current level
 *
 */
void print_level(int level_number)
{
    char label[3];
    sprintf(label,"%d",level_number);
    outtextxy(370,580,"LEVEL: ");
    outtextxy(420,580,label);
}


/** Function converts radians to degrees
 *
 * angle - angle in RAD
 *
 * Return
 * Degrees
 *
 */
double rad2deg(int angle)
{
    return angle*(PI/180);
}

/**Function that scale number to desired range
*Inputs: x - number to scale
*        input_min - min range of input number
*        input_max - max range of input number
*        out_min - min range of new scale
*        out_max - max range of new scale
*Return integer scaled number
*/
int scale_number(int x, int input_min, int input_max, int out_min, int out_max)
{
    return (int)(x-input_min)*(out_max-out_min)/(input_max-input_min)+out_min;
}

/**Random number generator
*Input: min - min boundary
*       max - max boundary
*Return double random number
*/
double rand_nmbr(double min, double max)
{
    return min + (double)(rand()/(double)RAND_MAX/(max - min + 1));
}


//-----------------------------------------------------------------------------
// This routine will be called by the PortAudio engine when audio is needed.
// It may called at interrupt level on some machines so don't do anything
// that could mess up the system like calling malloc() or free().
//-----------------------------------------------------------------------------
static int processingCallback( const void *inputBuffer, void *outputBuffer,
                               unsigned long framesPerBuffer,
                               const PaStreamCallbackTimeInfo* timeInfo,
                               PaStreamCallbackFlags statusFlags,
                               void *userData )
{
    USERDATA *data = (USERDATA*)userData;
    float *out = (float*)outputBuffer;
    unsigned long i;
    float aL,aR;
    (void) timeInfo; /* Prevent unused variable warnings. */
    (void) statusFlags;
    (void) inputBuffer;


    for( i=0; i<framesPerBuffer; i++ )
    {
        //Apply panning and volume
        aL=(1.0f-data->pan)*data->volume;
        aR=(1.0f-aL)*data->volume;

        *out++ = aL*data->drums.data[data->ridx];  /* left */
        *out++ = aR*data->drums.data[data->ridx];  /* right */

        data->ridx++;
        if(data->ridx>=data->drums.frames)
            data->ridx=0;
    }

    return paContinue;
}

//-----------------------------------------------------------------------------
//This routine is called by portaudio when playback is done.
//-----------------------------------------------------------------------------
static void audioStreamFinished( void* userData )
{
    USERDATA *data = (USERDATA *) userData;
//    printf( "Stream Completed: %s\n", data->message );
}

void main_audio(sounds_t sound_SFX)
{
    PaError err=0;
    char key;
    char ctrl;
    USERDATA data;
    AUDIOSTREAM outStream;
    int delay;
    char label[20];

    outStream.stream=NULL;
    outStream.sampleRate=44100;
    outStream.sampleFormat=paFloat32;
    outStream.inChannels=0;
    outStream.outChannels=2;
    outStream.framesPerBuffer=1024;
    data.ridx=0;
    data.volume=0.5;
    data.pan=0.5;

    switch(sound_SFX)
    {
    case INTRO:
        sprintf(label,"%s","samples/intro.wav");
        delay=1200;
        outStream.sampleRate=88200;
        break;
    case CROWD:
        sprintf(label,"%s","samples/hit_crowd.wav");
        delay=2000;
        outStream.sampleRate=16000;
        break;
    case ARROW_HIT:
        sprintf(label,"%s","samples/arrow_hit.wav");
        delay=601;
        outStream.sampleRate=44100;
        break;
    case ARROW_FLY:
        sprintf(label,"%s","samples/arrow_fly.wav");
        delay=253;
        outStream.sampleRate=44100;
        break;
    case BOW_PREPARATION:
        sprintf(label,"%s","samples/bow_prep.wav");
        delay=511;
        outStream.sampleRate=44100;
        break;
    case MISS_TARGET:
        sprintf(label,"%s","samples/miss_target.wav");
        delay=1600;
        outStream.sampleRate=44100;
        break;
    case STAY_FOCUSED:
        sprintf(label,"%s","samples/stay_focused.wav");
        delay=1200;
        outStream.sampleRate=44100;
        break;
    case GOOD:
        sprintf(label,"%s","samples/good.wav");
        delay=780;
        outStream.sampleRate=44100;
        break;
    case CLICK:
        sprintf(label,"%s","samples/click.wav");
        delay=100;
        outStream.sampleRate=88200;
        break;

    }
    err=wavread(label,&data.drums);
    err=initialiseAudioSystem();
    if(err!=0)
    {
        exit(EXIT_FAILURE);
    }
    err=openDefaultAudioStream(&outStream,processingCallback,&data);
    err=setAudioStreamFinishedCallback(&outStream,&audioStreamFinished);
    err=startAudioStream(&outStream);
    data.volume=1.0f;
    data.pan=0.5f;
    pausefor(delay);
    err=closeAudioStream(&outStream);
    err=terminateAudioSystem();

}




