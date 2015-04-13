#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <fstream>
#include <string>
#include <stdio.h>
#include <math.h>
#include "shuffle_generation.cpp"

using namespace std;

const double PI=3.1415926535897;

int colors[4][3]={{0,0,255},{255,0,0},{0,255,0},{255,255,0}};
int sticker_colors[4][9]= {{0,0,0,0,0,0,0,0,0},
                           {1,1,1,1,1,1,1,1,1},
                           {2,2,2,2,2,2,2,2,2},
                           {3,3,3,3,3,3,3,3,3}};
string moves[16]={"U ", "U' ", "L ", "L' ","R ", "R' ", "B ","B' ","u ","u' ","l ","l' ","r ","r' ","b ","b' "};
int *disp;
const int screen_Width=1000;
const int screen_Height=1000;
int centre[16][2]={{100,550},{200,550},{300,550},{400,550},{500,550},{600,550},{700,550},{800,550},
                  {100,600},{200,600},{300,600},{400,600},{500,600},{600,600},{700,600},{800,600}};
double endpoints[16][4]={{1.5,1.5,-1.5,1.5},{-1.5,1.5,1.5,1.5},{-1.5,1.5,0,3},{0,3,-1.5,1.5},
                         {0,3,1.5,1.5}  ,    {1.5,1.5,0,3}    ,  {0,1.5,0,3} ,   {0,3,0,1.5},
                         {0.5,0.5,-0.5,0.5},{-0.5,0.5,0.5,0.5},{-2.5,2.5,-2,3},{-2,3,-2.5,2.5},
                         {2,3,2.5,2.5} ,    {2.5,2.5,2,3}   ,  {0,2.25,0,3},   {0,3,0,2.25} };               
int x_origin=500;
int radius=15;
int start_x_origin=500;
int num_buttons=16;
int y_origin=0;
int start_y_origin=0;
int scale_coordinate_to_pixel=200;
double relative_coordinates_upper[6][2]={{0,0},{1,1},{-1,1},{2,2},{0,2},{-2,2}};
double relative_coordinates_lower[3][2]={{0,2},{1,3},{-1,3}};
double offset=0.08;
double length=1;
const float rectangle_y1=600;
const float rectangle_y2=662.5;
float rectangle_x1=45,rectangle_x2=105;
float startpoint_x=0,startpoint_y=0,endpoint_x=0,endpoint_y=0;
int arrow_counter=-1;
const float FPS = 2;
bool flag=true;
bool isback=false;
void draw_pyraminx_sticker(double, double, int, int);
void draw_pyraminx_face(int type);
void draw_arrow(int start_point_x ,int start_point_y ,int end_point_x ,int end_point_y,bool isBack);
int main(int argc, char **argv){
   
   
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
    
   ifstream infile("solved_moves.txt");

   int solved_moves[15];
   int solved_moves_counter=0;
   
   while(infile >> solved_moves[solved_moves_counter])
   {
      solved_moves_counter++;
   }

   string solved_move_sequence="";
         for (int i=0;i<solved_moves_counter;i++)
         {
            solved_move_sequence+=moves[solved_moves[i]]+" ";
         }
         
         arrow_counter++;
         startpoint_x=start_x_origin + endpoints[solved_moves[arrow_counter]][0]*0.5*scale_coordinate_to_pixel;
         startpoint_y=start_y_origin + endpoints[solved_moves[arrow_counter]][1]*sqrt(3)/2*scale_coordinate_to_pixel;
         endpoint_x  =start_x_origin + endpoints[solved_moves[arrow_counter]][2]*0.5*scale_coordinate_to_pixel;
         endpoint_y  =start_y_origin + endpoints[solved_moves[arrow_counter]][3]*sqrt(3)/2*scale_coordinate_to_pixel;
   
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
      else if (ev.type==ALLEGRO_EVENT_MOUSE_BUTTON_UP){
                if(arrow_counter==solved_moves_counter-1)
                  {
                  arrow_counter=0;
                  rectangle_x1=45;
                  rectangle_x2=105;
                  startpoint_x=start_x_origin + endpoints[solved_moves[arrow_counter]][0]*0.5*scale_coordinate_to_pixel;
                  startpoint_y=start_y_origin + endpoints[solved_moves[arrow_counter]][1]*sqrt(3)/2*scale_coordinate_to_pixel;
                  endpoint_x  =start_x_origin + endpoints[solved_moves[arrow_counter]][2]*0.5*scale_coordinate_to_pixel;
                  endpoint_y  =start_y_origin + endpoints[solved_moves[arrow_counter]][3]*sqrt(3)/2*scale_coordinate_to_pixel;
                  if(solved_moves[arrow_counter]==6||solved_moves[arrow_counter]==7||solved_moves[arrow_counter]==14||solved_moves[arrow_counter]==15)
                  isback=true;
               else isback=false;
                  }
                  else 
                {
                  arrow_counter++;
                rectangle_x1+=60;
               rectangle_x2+=60;
               startpoint_x=start_x_origin + endpoints[solved_moves[arrow_counter]][0]*0.5*scale_coordinate_to_pixel;
               startpoint_y=start_y_origin + endpoints[solved_moves[arrow_counter]][1]*sqrt(3)/2*scale_coordinate_to_pixel;
               endpoint_x  =start_x_origin + endpoints[solved_moves[arrow_counter]][2]*0.5*scale_coordinate_to_pixel;
               endpoint_y  =start_y_origin + endpoints[solved_moves[arrow_counter]][3]*sqrt(3)/2*scale_coordinate_to_pixel;
               if(solved_moves[arrow_counter]==6||solved_moves[arrow_counter]==7||solved_moves[arrow_counter]==14||solved_moves[arrow_counter]==15)
                  isback=true;
               else isback=false;

               if(arrow_counter==solved_moves_counter)
               {
                  cout << "yay" << endl;
                  arrow_counter=0;
                  rectangle_x1=600;
                  rectangle_x2=662.5;
                  startpoint_x=start_x_origin + endpoints[solved_moves[arrow_counter]][0]*0.5*scale_coordinate_to_pixel;
                  startpoint_y=start_y_origin + endpoints[solved_moves[arrow_counter]][1]*sqrt(3)/2*scale_coordinate_to_pixel;
                  endpoint_x  =start_x_origin + endpoints[solved_moves[arrow_counter]][2]*0.5*scale_coordinate_to_pixel;
                  endpoint_y  =start_y_origin + endpoints[solved_moves[arrow_counter]][3]*sqrt(3)/2*scale_coordinate_to_pixel;

               }
               }
            }
      if(redraw && al_is_event_queue_empty(event_queue)) {
         redraw = false;
         x_origin=start_x_origin;
         y_origin=start_y_origin;
         al_clear_to_color(al_map_rgb(0,0,0));
         draw_pyraminx_face(1);
         al_init_ttf_addon(); 
         ALLEGRO_FONT *font = al_load_ttf_font("Ubuntu-B.ttf",50,0 );
         
         draw_arrow(startpoint_x,startpoint_y,endpoint_x,endpoint_y,isback);
         al_draw_rectangle( rectangle_x1, rectangle_y1, rectangle_x2, rectangle_y2,  al_map_rgb(255,255,0), 1);
         al_draw_textf(font,al_map_rgb(255,255,255),50,y_origin+600, ALLEGRO_ALIGN_LEFT,"%s",&solved_move_sequence[0]);
         al_flip_display();
         al_shutdown_ttf_addon();
      }
   }
 
   al_destroy_timer(timer);
   al_destroy_display(display);
   al_destroy_event_queue(event_queue);
   
   return 0;
}
void draw_pyraminx_face(int type)
{
   
   for (int i=0;i<6;i++)
         {
            double x=relative_coordinates_upper[i][0];
            double y=relative_coordinates_upper[i][1];
            draw_pyraminx_sticker(x,y,0,type);
         }
         for (int i=0;i<3;i++)
         {
            double x=relative_coordinates_lower[i][0];
            double y=relative_coordinates_lower[i][1];
            draw_pyraminx_sticker(x,y,1,type);
         }  
}

void draw_pyraminx_sticker(double x,double y,int type,int facetype)
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
                                    al_map_rgb(255,255,255));
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
                                    al_map_rgb(255,255,255));
         }
}

void draw_arrow(int start_point_x ,int start_point_y ,int end_point_x,int end_point_y,bool isBack){
   float x1,y1,x2,y2,x3,y3;
   float a1,a2,b1,b2,length;
   a1=start_point_x;
   b1=start_point_y;
   a2=end_point_x;
   b2=end_point_y;
   length=sqrt((a1-a2)*(a1-a2)+(b1-b2)*(b1-b2));
   if(!isBack){  
   al_draw_line(start_point_x, start_point_y, end_point_x, end_point_y, al_map_rgb(255,0,0), 5);
   x1=  a2;
   y1=  b2;
   x2=  x1+((a1-a2)+(b1-b2))/length*sqrt(2)*20;
   y2=  y1+(-(a1-a2)+(b1-b2))/length*sqrt(2)*20;
   x3=  x1+((a1-a2)-(b1-b2))/length*sqrt(2)*20;
   y3=  y1+((a1-a2)+(b1-b2))/length*sqrt(2)*20;
   al_draw_filled_triangle(x1, y1, x2, y2, x3, y3, al_map_rgb(255,0,0));
   }
   else {
      al_draw_arc((start_point_x+end_point_x)/2,
                  (start_point_y+end_point_y)/2-100,
                  fabs((start_point_y-end_point_y)/2),
                  PI/2,fabs((start_point_y-end_point_y)/2)/((start_point_y-end_point_y)/2)*PI ,
                  al_map_rgb(255,0,0), 5);
               x1=a1+fabs((start_point_y-end_point_y)/2)/((start_point_y-end_point_y)/2)*25;
               y1=min(b2,b1)-100;
               x2=a2;
               y2=y1-25;
               x3=a2;
               y3=y1+25;
      al_draw_filled_triangle(x1, y1, x2, y2, x3, y3, al_map_rgb(255,0,0));

   }

}