/*
#include <pebble.h>

Window *window;
TextLayer *text_layer;

// init function
void init(){
  window = window_create();
  text_layer = text_layer_create(GRect(0,0,144,40));
  text_layer_set_text(text_layer, "Hello Pebble");
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));
  
  window_stack_push(window, true);
}

// clean-up function
void deinit(){
  window_destroy(window);
  text_layer_destroy(text_layer);
}

int f_main(){
  //init the app
  init();
  
  // wait for app events
  app_event_loop();
  
  // clean up the app
  deinit();
  
  // terminate normally
  return 0;
}
*/