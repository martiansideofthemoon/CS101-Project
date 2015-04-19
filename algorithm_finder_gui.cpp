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
int colors[6][3]={{0,0,255},{255,0,0},{0,255,0},{255,255,0},{255,255,255},{255,192,203}};
int sticker_colors[4][9]= {{0,0,0,0,0,0,0,0,0},
                           {1,1,1,1,1,1,1,1,1},
                           {2,2,2,2,2,2,2,2,2},
                           {3,3,3,3,3,3,3,3,3}}; // Sticker array
int sticker_displayed[4][9]= {{5,0,0,0,0,0,0,0,0},
                           {1,1,1,1,1,1,1,1,1},
                           {2,2,2,2,2,2,2,2,2},
                           {3,3,3,3,3,3,3,3,3}}; //Stickers displayed. Made to account for pink color
int active_coordinates[2]={0,0};
// string of moves to convert numbers into letters to show solution on the display
string moves[16]={"U ", "U' ", "L ", "L' ","R ", "R' ", "B ","B' ","u ","u' ","l ","l' ","r ","r' ","b ","b' "};
string button_text[8]={"Blue","Red","Green","Yellow","White","Left","Right","Done"};
string move_sequence="";
int colorid=0; // Color sticker
int *disp; 
const int screen_Width=1000;
const int screen_Height=1000;
int centre[8][2]={{100,550},{200,550},{300,550},{400,550},{500,550},{600,550},{700,550},{800,550}}; // Button coordinates
int x_origin=500; //Origin for drawing
int radius=25; //Radius of button
int font_size=12; 
int start_x_origin=500;
int num_buttons=8;
int y_origin=0;
int start_y_origin=0;
int scale_coordinate_to_pixel=100; //Scale factor
double relative_coordinates_upper[6][2]={{0,0},{1,1},{-1,1},{2,2},{0,2},{-2,2}}; //for upper stickers of a face
double relative_coordinates_lower[3][2]={{0,2},{1,3},{-1,3}}; //for lower stickers of a face
double offset=0.08;                                                              // Offset is half of the distance between
                                                                                 // the two pieces of a face of the pyraminx
                                                                                 // It basically helps create the black 
                                                                                 // space between pieces.
double length=1; //Coordinate length of sticker

const float FPS = 10; //Frames per second
bool flag=true;

void draw_pyraminx_sticker(double x,double y,int colorid,int type,int facetype);

void draw_pyraminx_face(int type,int face);

void output_pyraminx_state(); 
void draw_buttons();
void draw_button_labels();
void increment_active_coordinate(); //Go ahead in stickers
void decrement_active_coordinate(); //GO behind in stickers
void move_execute(int moveid);

int main(int argc, char **argv){
   /*
The main function operates by initializing the allegro variables.
The while(1) helps record events and then draws the various graphical elements as per the time event.
*/
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
   al_init_font_addon(); // initialize font addons 
   //al_init_ttf_addon(); 
   al_install_mouse(); // // initialize mouse
   event_queue = al_create_event_queue();
   if(!event_queue) {
      fprintf(stderr, "failed to create event_queue!\n");
      al_destroy_display(display);
      al_destroy_timer(timer);
      return -1;
   }
   // register the display,timer and mouse events in the event queue.
   al_register_event_source(event_queue, al_get_display_event_source(display));
   
   al_register_event_source(event_queue, al_get_timer_event_source(timer));
   al_register_event_source(event_queue, al_get_mouse_event_source());
 
   al_clear_to_color(al_map_rgb(0,0,0)); // makes screen black
   // allegro works with two screens one behind the other it makes changes in the back screen hence
   // flip display is used to show the changes that have occured since it flips and shows the back screen
   al_flip_display();
 
   al_start_timer(timer);
   

   while(1)
   {
      ALLEGRO_EVENT ev;
      al_wait_for_event(event_queue, &ev);
 
      if(ev.type == ALLEGRO_EVENT_TIMER) {
         
         redraw = true; //Draw screen every time this function is called. FPS concept
      }
      else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
         output_pyraminx_state(); //write to file
         flag=true;
         break;
      }
      else if (ev.type==ALLEGRO_EVENT_MOUSE_BUTTON_UP)
      {
         int mouse_x=ev.mouse.x;
         int mouse_y=ev.mouse.y;
         bool flag=false;
         for (int i=0;i<num_buttons;i++)
         { //Check which button was pressed
            if (sqrt((mouse_x-centre[i][0])*(mouse_x-centre[i][0])+(mouse_y-centre[i][1])*(mouse_y-centre[i][1]))<radius)
            {
               if (i<=4)
               {
                  sticker_colors[active_coordinates[0]][active_coordinates[1]]=i; //Put corresponding color in arrays
                  sticker_displayed[active_coordinates[0]][active_coordinates[1]]=i;
                  increment_active_coordinate(); //Move pink sticker forward
               }
               else if (i==5)
               {
                  decrement_active_coordinate();
               }
               else if (i==6)
               {
                  increment_active_coordinate();
               }
               else if (i==7)
               {
                  output_pyraminx_state(); //Write to file
                  flag=true;
               }
            }
         }
         if (flag) break;
      }
      if(redraw && al_is_event_queue_empty(event_queue)) {
         redraw = false;
         x_origin=start_x_origin;
         y_origin=start_y_origin;
         al_clear_to_color(al_map_rgb(0,0,0));
         draw_buttons();
         draw_button_labels();
         draw_pyraminx_face(1,0);
         x_origin+=length*3*scale_coordinate_to_pixel; //Move origin
         draw_pyraminx_face(1,1);
         x_origin-=2*length*3*scale_coordinate_to_pixel;
         draw_pyraminx_face(1,2);
         x_origin=start_x_origin;
         y_origin+=3*length*sqrt(3)*scale_coordinate_to_pixel;
         draw_pyraminx_face(-1,3);  
         y_origin+=length*scale_coordinate_to_pixel;
         /*al_init_ttf_addon(); 
         ALLEGRO_FONT *font = al_load_ttf_font("Ubuntu-B.ttf",32,0 );
         string shuffle_move_sequence="";
         for (int i=0;i<15;i++)
         {
            shuffle_move_sequence+=moves[disp[i]];
         }
         al_draw_textf(font,al_map_rgb(255,255,255),(double)screen_Width/2,y_origin, ALLEGRO_ALIGN_CENTRE,"%s",&shuffle_move_sequence[0]);
         al_flip_display();
         al_shutdown_ttf_addon();*/
         al_flip_display();

      }
      //output_move_sequence();
   }
 
   al_destroy_timer(timer);
   al_destroy_display(display);
   al_destroy_event_queue(event_queue);
   
   return 0;
}
void draw_pyraminx_face(int type,int face)
{
   /*
draws each face of pyraminx
type is facetype, 1 for first three faces, -1 for last face
face is face number
*/
   //Using coordinate geometry to draw pyraminx face
   int upper_sticker_order[6];
   int lower_sticker_order[3];
   if (face!=3)
   { //Changing order to match data order
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
            draw_pyraminx_sticker(x,y,sticker_displayed[face][upper_sticker_order[i]],0,type);
         }
         for (int i=0;i<3;i++)
         {
            double x=relative_coordinates_lower[i][0];
            double y=relative_coordinates_lower[i][1];
            draw_pyraminx_sticker(x,y,sticker_displayed[face][lower_sticker_order[i]],1,type);
         }  
}

void draw_pyraminx_sticker(double x,double y,int colorid,int type,int facetype)
{
   /*
Draws the sticker of the pyraminx using coordinate geometry. Its parameters are 
x --> relative x coordinate of the sticker position
y --> relative y coordinate of the sticker position
colorid --> color to be drawn using the standard scheme (blue is 0, red is 1, green is 2, yellow is 3, white is 4, pink is 5)
type --> upward pointing or downward pointing are 1 and -1 respectively
facetype --> first three faces are type 1, fourth face is type -1
*/
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
void output_pyraminx_state()
{
   /*
Write to file test.txt
*/
   //move_execute(16);
   ofstream ofile("test.txt");
   for (int i=0;i<4;i++)
   {
      for (int j=0;j<9;j++)
      {
         ofile << sticker_colors[i][j];
      }
      ofile << endl;
   }
   ofile.close();
}
void draw_buttons()
{ 
   /*
   Draw all the buttons on the screen
   */
   for (int i=0;i<num_buttons;i++)
   {
      al_draw_filled_circle(centre[i][0], centre[i][1], radius, al_map_rgb(255,0,0));
   }
}
void draw_button_labels()
{
   /*
   Draw all the button stickers centered in the buttons
   */
         al_init_ttf_addon(); 
         ALLEGRO_FONT *font = al_load_ttf_font("Ubuntu-B.ttf",font_size,0 ); //Load this font
         for (int i=0;i<num_buttons;i++)
         {
            al_draw_textf(font,al_map_rgb(255,255,255),centre[i][0]+2,centre[i][1]-10, ALLEGRO_ALIGN_CENTRE,"%s",&button_text[i][0]);
         }
         al_shutdown_ttf_addon();
}
void increment_active_coordinate()
{
   /*
   Move the pink sticker one ahead
   */
   int face=active_coordinates[0]; //Face number of pink
   int sticker=active_coordinates[1]; //Sticker number of pink
   sticker++;
   if (sticker>=9)
   {
      sticker=0; 
      face++;
   }
   if (face>=4)
   {
      face=0;
      sticker=0;
   }
   sticker_displayed[active_coordinates[0]][active_coordinates[1]]=sticker_colors[active_coordinates[0]][active_coordinates[1]];
   sticker_displayed[face][sticker]=5;
   active_coordinates[0]=face;
   active_coordinates[1]=sticker;
}
void decrement_active_coordinate()
{
   /*
   Move the pink sticker one behind
   */
   int face=active_coordinates[0];
   int sticker=active_coordinates[1];
   sticker--;
   if (sticker<0)
   {
      sticker=8;
      face--;
   }
   if (face<0)
   {
      face=3;
      sticker=8;
   }
   sticker_displayed[active_coordinates[0]][active_coordinates[1]]=sticker_colors[active_coordinates[0]][active_coordinates[1]];
   sticker_displayed[face][sticker]=5;
   active_coordinates[0]=face;
   active_coordinates[1]=sticker;
}