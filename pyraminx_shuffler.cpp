#include "allegro5/allegro.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <fstream>
#include <string>
#include <stdio.h>
#include <math.h>
#include "shuffle_generation.cpp"

using namespace std;
int colors[4][3]={{0,0,255},{255,0,0},{0,255,0},{255,255,0}};
int sticker_colors[4][9]= {{0,0,0,0,0,0,0,0,0},
                           {1,1,1,1,1,1,1,1,1},
                           {2,2,2,2,2,2,2,2,2},
                           {3,3,3,3,3,3,3,3,3}};
string moves[16]={"U ", "U' ", "L ", "L' ","R ", "R' ", "B ","B' ","u ","u' ","l ","l' ","r ","r' ","b ","b' "};
int colorid=0;
int *disp;
int x_origin=500;
int start_x_origin=500;
int y_origin=0;
int start_y_origin=0;
int scale_coordinate_to_pixel=100;
double relative_coordinates_upper[6][2]={{0,0},{1,1},{-1,1},{2,2},{0,2},{-2,2}};
double relative_coordinates_lower[3][2]={{0,2},{1,3},{-1,3}};
double offset=0.08;
double length=1;
const int screen_Width=1000;
const int screen_Height=1000;
const float FPS = 60;
bool flag=true;
void draw_pyraminx_sticker(double, double, int, int, int);
void draw_pyraminx_face(int type,int face);
void shuffle();
void output_move_sequence();
int main(int argc, char **argv){
   shuffle();
   ALLEGRO_DISPLAY *display = NULL;
   ALLEGRO_EVENT_QUEUE *event_queue = NULL;
   ALLEGRO_TIMER *timer = NULL;
   bool redraw = true;
   
   if(!al_init()) {
      fprintf(stderr, "failed to initialize allegro!\n");
      return -1;
   }
 
   timer = al_create_timer(1.0 / FPS);
   if(!timer) {
      fprintf(stderr, "failed to create timer!\n");
      return -1;
   }
 
   display = al_create_display(screen_Width, screen_Height);
   if(!display) {
      fprintf(stderr, "failed to create display!\n");
      al_destroy_timer(timer);
      return -1;
   }
   al_init_font_addon();
   al_init_ttf_addon(); 
   event_queue = al_create_event_queue();
   if(!event_queue) {
      fprintf(stderr, "failed to create event_queue!\n");
      al_destroy_display(display);
      al_destroy_timer(timer);
      return -1;
   }
 
   al_register_event_source(event_queue, al_get_display_event_source(display));
 
   al_register_event_source(event_queue, al_get_timer_event_source(timer));
 
   al_clear_to_color(al_map_rgb(0,0,0));
 
   al_flip_display();
 
   al_start_timer(timer);
   

   while(1)
   {
      ALLEGRO_EVENT ev;
      al_wait_for_event(event_queue, &ev);
 
      if(ev.type == ALLEGRO_EVENT_TIMER) {
         redraw = true;
      }
      else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
         break;
      }
 
      if(redraw && al_is_event_queue_empty(event_queue)) {
         redraw = false;
         x_origin=start_x_origin;
         y_origin=start_y_origin;
         al_clear_to_color(al_map_rgb(0,0,0));
         
         draw_pyraminx_face(1,0);
         x_origin+=length*3*scale_coordinate_to_pixel;
         draw_pyraminx_face(1,1);
         x_origin-=2*length*3*scale_coordinate_to_pixel;
         draw_pyraminx_face(1,2);
         x_origin=start_x_origin;
         y_origin+=3*length*sqrt(3)*scale_coordinate_to_pixel;
         draw_pyraminx_face(-1,3);  
         y_origin+=length*scale_coordinate_to_pixel;
         ALLEGRO_FONT *font = al_load_ttf_font("Ubuntu-B.ttf",32,0 );
         string move_sequence="";
         for (int i=0;i<15;i++)
         {
            move_sequence+=moves[disp[i]];
         }
         al_draw_text(font,al_map_rgb(255,255,255),(double)screen_Width/2,y_origin, ALLEGRO_ALIGN_CENTRE,&move_sequence[0]);
         al_flip_display();

      }
      output_move_sequence();
   }
 
   al_destroy_timer(timer);
   al_destroy_display(display);
   al_destroy_event_queue(event_queue);
   
   return 0;
}
void draw_pyraminx_face(int type,int face)
{
   int upper_sticker_order[6];
   int lower_sticker_order[3];
   if (face!=3)
   {
   upper_sticker_order[0]=0;
   upper_sticker_order[1]=3;
   upper_sticker_order[2]=1;
   upper_sticker_order[3]=8;
   upper_sticker_order[4]=6;
   upper_sticker_order[5]=4;
   lower_sticker_order[0]=2;
   lower_sticker_order[1]=7;
   lower_sticker_order[2]=5;
   }
   else
   {
   upper_sticker_order[0]=8;
   upper_sticker_order[1]=3;
   upper_sticker_order[2]=6;
   upper_sticker_order[3]=0;
   upper_sticker_order[4]=1;
   upper_sticker_order[5]=4;
   lower_sticker_order[0]=7;
   lower_sticker_order[1]=2;
   lower_sticker_order[2]=5;
   }
   for (int i=0;i<6;i++)
         {
            double x=relative_coordinates_upper[i][0];
            double y=relative_coordinates_upper[i][1];
            draw_pyraminx_sticker(x,y,sticker_colors[face][upper_sticker_order[i]],0,type);
         }
         for (int i=0;i<3;i++)
         {
            double x=relative_coordinates_lower[i][0];
            double y=relative_coordinates_lower[i][1];
            draw_pyraminx_sticker(x,y,sticker_colors[face][lower_sticker_order[i]],1,type);
         }  
}
void shuffle()
{
   disp=final_display();
   for (int i=0;i<15;i++)
   {
      int adj[16][4][9];
      adjacency(sticker_colors,adj);
      for (int a=0;a<4;a++)
      {
         for (int b=0;b<9;b++)
            sticker_colors[a][b]=adj[disp[i]][a][b];
      }
   }
}
void draw_pyraminx_sticker(double x,double y,int colorid,int type,int facetype)
{
            if(type==0)
            {
            x=x*0.5*length;
            y=y*(sqrt(3)/2)*length;
            double coordinates[3][2]={{0,0},{0,0},{0,0}};
            coordinates[0][0]=x-length/2+sqrt(3)*offset;
            coordinates[0][1]=y+sqrt(3)*length/2-offset;
            coordinates[1][0]=x+length/2-sqrt(3)*offset;
            coordinates[1][1]=y+sqrt(3)*length/2-offset;
            coordinates[2][0]=x;
            coordinates[2][1]=y+2*offset;
            
            al_draw_filled_triangle(x_origin+coordinates[0][0]*scale_coordinate_to_pixel,
                                    y_origin+facetype*coordinates[0][1]*scale_coordinate_to_pixel,
                                    x_origin+coordinates[1][0]*scale_coordinate_to_pixel,
                                    y_origin+facetype*coordinates[1][1]*scale_coordinate_to_pixel,
                                    x_origin+coordinates[2][0]*scale_coordinate_to_pixel,
                                    y_origin+facetype*coordinates[2][1]*scale_coordinate_to_pixel,
                                    al_map_rgb(colors[colorid][0],colors[colorid][1],colors[colorid][2]));
         }
         else if (type==1)
         {
            x=x*0.5*length;
            y=y*(sqrt(3)/2)*length;
            double coordinates[3][2]={{0,0},{0,0},{0,0}};
            coordinates[0][0]=x-length/2+sqrt(3)*offset;
            coordinates[0][1]=y-(sqrt(3)*length/2)+offset;
            coordinates[1][0]=x+length/2-sqrt(3)*offset;
            coordinates[1][1]=y-(sqrt(3)*length/2)+offset;
            coordinates[2][0]=x;
            coordinates[2][1]=y-2*offset;
            
            al_draw_filled_triangle(x_origin+coordinates[0][0]*scale_coordinate_to_pixel,
                                    y_origin+facetype*coordinates[0][1]*scale_coordinate_to_pixel,
                                    x_origin+coordinates[1][0]*scale_coordinate_to_pixel,
                                    y_origin+facetype*coordinates[1][1]*scale_coordinate_to_pixel,
                                    x_origin+coordinates[2][0]*scale_coordinate_to_pixel,
                                    y_origin+facetype*coordinates[2][1]*scale_coordinate_to_pixel,
                                    al_map_rgb(colors[colorid][0],colors[colorid][1],colors[colorid][2]));
         }
}
void output_move_sequence()
{
   ofstream ofile("Analysis.txt");
   for (int i=0;i<15;i++)
      ofile << disp[i] << " ";
   ofile.close();
}