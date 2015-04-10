#include <allegro5/allegro_image.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>
#include <stdio.h>
using namespace std;
int colors[4][3]={{0,0,255},{255,0,0},{0,255,0},{255,255,0}};
int colorid=0;

const int screen_Width=1000;
const int screen_Height=1000;
const float FPS = 60;
 
int main(int argc, char **argv){
 
   ALLEGRO_DISPLAY *display = NULL;
   ALLEGRO_EVENT_QUEUE *event_queue = NULL;
   ALLEGRO_TIMER *timer = NULL;
   ALLEGRO_BITMAP  *image   = NULL;
   bool redraw = true;
 	display = al_create_display(screen_Width, screen_Height);
 	timer = al_create_timer(1.0 / FPS);
   event_queue = al_create_event_queue();
   if(!al_init()) {
      cout <<"failed to initialize allegro!\n";
      return -1;
   }
 	if(!al_init_image_addon()) {
      cout << "Failed to load image addon\n" ;
      return 0;
   }
   if(!timer) {
      cout << "failed to create timer!\n";
      return -1;
   }
   if(!display) {
      cout << "failed to create display!\n";
      al_destroy_timer(timer);
      return -1;
   }
   if(!event_queue) {
      cout << "failed to create event_queue!\n";
      al_destroy_display(display);
      al_destroy_timer(timer);
      return -1;
   }
 	image = al_load_bitmap("apple.jpg");
 	if(!image) {
      cout << "Failed to load image\n"; 
      al_destroy_display(display);
      return 0;
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
         al_clear_to_color(al_map_rgb(0,0,0));
         al_draw_bitmap(image,200,200,0);
         al_flip_display();
      }
   }
 	al_destroy_bitmap(image);
   al_destroy_timer(timer);
   al_destroy_display(display);
   al_destroy_event_queue(event_queue);
 
   return 0;
}