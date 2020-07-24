#ifndef ARCHERY_H_INCLUDED
#define ARCHERY_H_INCLUDED


#define X_WINDOW 800
#define Y_WINDOW 600
#define X_TARGET 700
#define Y_TARGET 500
#define X_BOW_CENTER 50
#define Y_BOW_CENTER 500
#define TREE_NUMBER 50
#define ARROW_NUMBER 3
#define LEVEL_NUMBER 10

typedef enum
{
    INTRO=0,
    CROWD,
    ARROW_HIT,
    ARROW_FLY,
    BOW_PREPARATION,
    MISS_TARGET,
    GOOD,
    STAY_FOCUSED,
    CLICK
}sounds_t;

void welcome_screen(int *x_p, int *y_p);
void landscape();
void trees(int x_pos, int y_pos, float scale, int thickness);
void tents();
void draw_forest(int trees_number);
void target();
void draw_bow(int y_position, int *x_p, int *y_p, int *angle);
void draw_player();
void draw_strength_bar(int x_pos);
int score_value(int score_pos);
void print_score(int score);
void print_hit_miss(int sel);
void print_arrow_left(int arrow_count);
void print_wind(int wind, char wind_dir);
void print_level(int level_number);
int draw_arrow_path(int x_start_projectile, int y_start_projectile,
                            int velocity, int angle,int wind, char wind_dir, int *hit_miss);
double rad2deg(int angle);
void main_audio(sounds_t sound_SFX);
int end_screen(int *x_p, int *y_p, int score, int level);
int scale_number(int x, int input_min, int input_max, int out_min, int out_max);
double rand_nmbr(double min, double max);

#endif // ARCHERY_H_INCLUDED

