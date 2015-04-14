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
string move_sequence="";
int colorid=0;
int *disp;
const int screen_Width=1000;
const int screen_Height=1000;
int centre[16][2]={{100,550},{200,550},{300,550},{400,550},{500,550},{600,550},{700,550},{800,550},
                  {100,600},{200,600},{300,600},{400,600},{500,600},{600,600},{700,600},{800,600}};
int x_origin=500;
int radius=15;
int start_x_origin=500;
int num_buttons=16;
int y_origin=0;
int start_y_origin=0;
int scale_coordinate_to_pixel=100;
double relative_coordinates_upper[6][2]={{0,0},{1,1},{-1,1},{2,2},{0,2},{-2,2}};
double relative_coordinates_lower[3][2]={{0,2},{1,3},{-1,3}};
double offset=0.08;
double length=1;

const float FPS = 10;
bool flag=true;
void draw_pyraminx_sticker(double, double, int, int, int);
void draw_pyraminx_face(int type,int face);
void shuffle();
void output_move_sequence();
void draw_buttons();
void draw_button_labels();
void move_execute(int moveid);
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
   al_install_mouse();
   event_queue = al_create_event_queue();
   if(!event_queue) {
      fprintf(stderr, "failed to create event_queue!\n");
      al_destroy_display(display);
      al_destroy_timer(timer);
      return -1;
   }
 
   al_register_event_source(event_queue, al_get_display_event_source(display));
   
   al_register_event_source(event_queue, al_get_timer_event_source(timer));
   al_register_event_source(event_queue, al_get_mouse_event_source());
 
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
      else if (ev.type==ALLEGRO_EVENT_MOUSE_BUTTON_UP)
      {
         int mouse_x=ev.mouse.x;
         int mouse_y=ev.mouse.y;
         for (int i=0;i<num_buttons;i++)
         {
            if (sqrt((mouse_x-centre[i][0])*(mouse_x-centre[i][0])+(mouse_y-centre[i][1])*(mouse_y-centre[i][1]))<radius)
            {
               move_execute(i);
               break;
            }
         }
            //move_execute(0);
      }
      if(redraw && al_is_event_queue_empty(event_queue)) {
         redraw = false;
         x_origin=start_x_origin;
         y_origin=start_y_origin;
         al_clear_to_color(al_map_rgb(0,0,0));
         draw_buttons();
         draw_button_labels();
         draw_pyraminx_face(1,0);
         x_origin+=length*3*scale_coordinate_to_pixel;
         draw_pyraminx_face(1,1);
         x_origin-=2*length*3*scale_coordinate_to_pixel;
         draw_pyraminx_face(1,2);
         x_origin=start_x_origin;
         y_origin+=3*length*sqrt(3)*scale_coordinate_to_pixel;
         draw_pyraminx_face(-1,3);  
         y_origin+=length*scale_coordinate_to_pixel;
         al_init_ttf_addon(); 
         ALLEGRO_FONT *font = al_load_ttf_font("Ubuntu-B.ttf",32,0 );
         string shuffle_move_sequence="";
         for (int i=0;i<15;i++)
         {
            shuffle_move_sequence+=moves[disp[i]];
         }
         al_draw_textf(font,al_map_rgb(255,255,255),(double)screen_Width/2,y_origin, ALLEGRO_ALIGN_CENTRE,"%s",&shuffle_move_sequence[0]);
         al_flip_display();
         al_shutdown_ttf_addon();

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
   //move_execute(12);
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
   //move_execute(16);
   ofstream ofile("Analysis.txt");
   for (int i=0;i<15;i++)
      ofile << disp[i] << " ";
   ofile << "\n" << move_sequence;
   ofile.close();
}
void move_execute(int moveid)
{
      int adj[16][4][9];
      adjacency(sticker_colors,adj);
      for (int a=0;a<4;a++)
      {
         for (int b=0;b<9;b++)
            sticker_colors[a][b]=adj[moveid][a][b];
      }
      if (moveid>=10)
      {
      move_sequence+=(char)((moveid/10)+48);
      move_sequence+=(char)((moveid%10)+48);
      }
      else
      move_sequence+=((char)moveid+48);
      move_sequence+=" ";
}
void draw_buttons()
{
   for (int i=0;i<16;i++)
   {
      al_draw_filled_circle(centre[i][0], centre[i][1], radius, al_map_rgb(255,0,0));
   }
}
void draw_button_labels()
{
         al_init_ttf_addon(); 
         ALLEGRO_FONT *font = al_load_ttf_font("Ubuntu-B.ttf",20,0 );
         for (int i=0;i<16;i++)
         {
            al_draw_textf(font,al_map_rgb(255,255,255),centre[i][0]+2,centre[i][1]-10, ALLEGRO_ALIGN_CENTRE,"%s",&moves[i][0]);
         }
         al_shutdown_ttf_addon();
}