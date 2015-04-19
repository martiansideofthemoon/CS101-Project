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
// string of moves to convert numbers into letters to show solution on the display
string moves[16]={"U ", "U' ", "L ", "L' ","R ", "R' ", "B ","B' ","u ","u' ","l ","l' ","r ","r' ","b ","b' "}; 
const int screen_Width=1000;
const int screen_Height=1000;
double endpoints[16][4]={{1.5,1.5,-1.5,1.5},{-1.5,1.5,1.5,1.5},{-1.5,1.5,0,3},{0,3,-1.5,1.5},
                         {0,3,1.5,1.5}  ,    {1.5,1.5,0,3}    ,  {0,1.5,0,3} ,   {0,3,0,1.5},
                         {0.5,0.5,-0.5,0.5},{-0.5,0.5,0.5,0.5},{-2.5,2.5,-2,3},{-2,3,-2.5,2.5},
                         {2,3,2.5,2.5} ,    {2.5,2.5,2,3}   ,  {0,2.25,0,3},   {0,3,0,2.25} };                
int x_origin=500;                                                                // To specify the current x_origin.
int start_x_origin=500;
int y_origin=0;                                                                  // To specify the current y_origin.
int start_y_origin=0;
int scale_coordinate_to_pixel=200;
double relative_coordinates_upper[6][2]={{0,0},{1,1},{-1,1},{2,2},{0,2},{-2,2}}; // To specify relative coordinates 
                                                                                 // of the traingles pointing upward.
double relative_coordinates_lower[3][2]={{0,2},{1,3},{-1,3}};                    // To specify relatives coordinates
                                                                                 // of the traingles pointing downward.   
double offset=0.08;                                                              // Offset is half of the distance between
                                                                                 // the two pieces of a face of the pyraminx
                                                                                 // It basically helps create the black 
                                                                                 // space between pieces. 
double length=1;                                                                 // Length is the relative length of the
                                                                                 // side of a piece we have assumed to 
                                                                                 // find the coordinates.
const float rectangle_y1=600;                                                    // Y-Coordinates of the diagonal of the  
const float rectangle_y2=662.5;                                                  // rectangle enclosing the text
float rectangle_x1=45,rectangle_x2=95;                                           // X-coordinate of the above rectangle
float startpoint_x=0,startpoint_y=0,endpoint_x=0,endpoint_y=0;                   // start and endpoints of arrow.
int arrow_counter=-1;
const float FPS = 2;                                                             // Frame per second.
bool flag=true;
bool isback=false;                                                               // To check whether the move is back.
void draw_pyraminx_sticker(double, double, int, int);
void draw_pyraminx_face(int type);
void draw_arrow(int start_point_x ,int start_point_y ,int end_point_x ,int end_point_y,bool isBack);
int main(int argc, char **argv){
   /*
The main function operates by initializing the allegro variables.
The while(1) helps record events and then draws the various graphical elements as per the time event.
*/
   // creating display event_queue timer variables.
   ALLEGRO_DISPLAY *display = NULL;
   ALLEGRO_EVENT_QUEUE *event_queue = NULL;
   ALLEGRO_TIMER *timer = NULL;
   bool redraw = true;
   
   if(!al_init()) {
      fprintf(stderr, "failed to initialize allegro!\n");
      return -1;
   }
 
   timer = al_create_timer(1.0 / FPS);                         // starts the timer
   if(!timer) {
      fprintf(stderr, "failed to create timer!\n");
      return -1;
   }
 
   display = al_create_display(screen_Width, screen_Height);   // starts the display
   if(!display) {
      fprintf(stderr, "failed to create display!\n");
      al_destroy_timer(timer);
      return -1;
   }
   al_init_font_addon();                                       // initialize font addons 
   al_init_ttf_addon();                                        // initialize true-type font addons
   al_install_mouse();                                         // initialize mouse
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
 
   al_clear_to_color(al_map_rgb(0,0,0));                // makes screen black
   // allegro works with two screens one behind the other it makes changes in the back screen hence
   // flip display is used to show the changes that have occured since it flips and shows the back screen
   al_flip_display();                                                                    

   al_start_timer(timer);
    
   ifstream infile("solved_moves.txt"); // takes input from the specified file

   int solved_moves[15];                // used to tore the solution in integer form
   int solved_moves_counter=0;
   
   while(infile >> solved_moves[solved_moves_counter])
   {
      solved_moves_counter++;
   }

   string solved_move_sequence="";
         for (int i=0;i<solved_moves_counter;i++)
         {
            solved_move_sequence+=moves[solved_moves[i]]+" "; // converts numbers into a string of the solution.
         }
         // here the startpoint and endpoint of the arrow are initialized using appropriate coordinate geometry.
         arrow_counter++;
         startpoint_x=start_x_origin + endpoints[solved_moves[arrow_counter]][0]*0.5*scale_coordinate_to_pixel;
         startpoint_y=start_y_origin + endpoints[solved_moves[arrow_counter]][1]*sqrt(3)/2*scale_coordinate_to_pixel;
         endpoint_x  =start_x_origin + endpoints[solved_moves[arrow_counter]][2]*0.5*scale_coordinate_to_pixel;
         endpoint_y  =start_y_origin + endpoints[solved_moves[arrow_counter]][3]*sqrt(3)/2*scale_coordinate_to_pixel;
         if(solved_moves[arrow_counter]==6||solved_moves[arrow_counter]==7||solved_moves[arrow_counter]==14||solved_moves[arrow_counter]==15)
                  isback=true;
               else isback=false;
   
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
       if(arrow_counter==solved_moves_counter-1)         // this if condition is to bring the rectangle back to the first 
         {                                               // position enclosing the first move
         arrow_counter=0;
         // initializes the coordinates of the rectangle that encloses the the text.
         rectangle_x1=45;
         rectangle_x2=95;
         // here the startpoint and endpoint of the arrow are changed using appropriate coordinate geometry.
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
      // initializes the coordinates of the rectangle that encloses the the text.
      rectangle_x1+=57;
      rectangle_x2+=57;
      // here the startpoint and endpoint of the arrow are changed using appropriate coordinate geometry.
      startpoint_x=start_x_origin + endpoints[solved_moves[arrow_counter]][0]*0.5*scale_coordinate_to_pixel;
      startpoint_y=start_y_origin + endpoints[solved_moves[arrow_counter]][1]*sqrt(3)/2*scale_coordinate_to_pixel;
      endpoint_x  =start_x_origin + endpoints[solved_moves[arrow_counter]][2]*0.5*scale_coordinate_to_pixel;
      endpoint_y  =start_y_origin + endpoints[solved_moves[arrow_counter]][3]*sqrt(3)/2*scale_coordinate_to_pixel;
      if(solved_moves[arrow_counter]==6||solved_moves[arrow_counter]==7||solved_moves[arrow_counter]==14||solved_moves[arrow_counter]==15)
         isback=true;
      else isback=false;
      }
   }
      if(redraw && al_is_event_queue_empty(event_queue)) {
         redraw = false;
         x_origin=start_x_origin;            // set current origin to initial origin
         y_origin=start_y_origin;                        
         al_clear_to_color(al_map_rgb(0,0,0));     // makes whole screen black
         draw_pyraminx_face(1);                    // draws one pyramix face
         al_init_ttf_addon();                      // adds the font to the allegro application
         ALLEGRO_FONT *font = al_load_ttf_font("Ubuntu-B.ttf",50,0 );   // loads the given font in the font.
         
         draw_arrow(startpoint_x,startpoint_y,endpoint_x,endpoint_y,isback);
         al_draw_rectangle( rectangle_x1, rectangle_y1, rectangle_x2, rectangle_y2,  al_map_rgb(255,255,0), 1);
         al_draw_textf(font,al_map_rgb(255,255,255),50,y_origin+600, ALLEGRO_ALIGN_LEFT,"%s",&solved_move_sequence[0]);
         al_flip_display();
         al_shutdown_ttf_addon();
      }
   }
   // destroy timer, display & event queue in the end.
   al_destroy_timer(timer);
   al_destroy_display(display);
   al_destroy_event_queue(event_queue);
   
   return 0;
}
void draw_pyraminx_face(int type)
{
   /*
draws each face of pyraminx
type is facetype, 1 for first three faces, -1 for last face
face is face number
*/
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
   /*
Draws the sticker of the pyraminx using coordinate geometry. Its parameters are 
x --> relative x coordinate of the sticker position
y --> relative y coordinate of the sticker position
colorid --> color to be drawn using the standard scheme (blue is 0, red is 1, green is 2, yellow is 3, white is 4, pink is 5)
type --> upward pointing or downward pointing are 1 and -1 respectively
facetype --> first three faces are type 1, fourth face is type -1
*/
            // The coordinates of the stickers are determined using appopriate coordinate geometry
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
   /*
   This function draws an arrow on allegro.
   start_point_x --> Starting x coordinate
   start_point_y --> Starting y coordinate
   end_point_x --> Ending x coordinate
   end_point_y --> Ending y coordinate
   isBack --> Is move B,B',b,b'?
   */
   // the coordinates of the triangle at the end of the arrow are determined by using appropriate coordinate geometry.

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