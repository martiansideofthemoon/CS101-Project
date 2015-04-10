#include "allegro5/allegro.h"
#include <allegro5/allegro_primitives.h>
int colors[3][3]={{0,0,255},{255,0,0},{0,255,0},{255,255,0}};
int colorid=0;
int main(int argc, char **argv){
 
   ALLEGRO_DISPLAY *display = NULL;
 
   if(!al_init()) {
      //fprintf(stderr, "failed to initialize allegro!\n");
      return -1;
   }
 
   display = al_create_display(640, 480);
   if(!display) {
      //fprintf(stderr, "failed to create display!\n");
      return -1;
   }
 while (1)
 {

   al_clear_to_color(al_map_rgb(0,0,0));
 	//al_draw_line(100, 100, 300, 300, al_map_rgb(255,0,0), 1);
 	al_draw_filled_triangle(100, 100, 100,50,
   200,50, al_map_rgb(colors[colorid][0],colors[colorid][1],colors[colorid][2]));
   al_flip_display();
   al_rest(1.0);
   colorid++;
   if (colorid>2) colorid=0;
 }
   al_destroy_display(display);
 
   return 0;
}