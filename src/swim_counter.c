#include <pebble.h>

// Functions
void update_text_layers();
void init_text_layers();

static void select_click_handler(ClickRecognizerRef recognizer, void *context);
static void up_click_handler(ClickRecognizerRef recognizer, void *context);
static void down_click_handler(ClickRecognizerRef recognizer, void *context);
static void click_config_provider(void *context);

const int LUNGHEZZA_VASCA = 2 * 50;
Window *window;
TextLayer *text_layer_counter;
TextLayer *text_layer_style;
TextLayer *text_layer_total;

// contatore delle vasche
int counter = 0;
// TBWFM - andrebbe allocata dinamicamente
char counter_string[2];

// indicazione dello stile
int current_style_index;
const char  *styles [] = {"Rana", "Stile", "Gambe Dorso", "Braccia Stile"};
int number_of_styles;

// distanza totale percorsa
int total = 0;
// TBWFM - andrebbe allocata dinamicamente
char total_string[9];

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  //text_layer_set_text(text_layer, "Select");
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  //text_layer_set_text(text_layer, "Up");
  counter = counter + 1;
  update_text_layers();
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  //text_layer_set_text(text_layer, "Down");
  current_style_index = (current_style_index + 1) % number_of_styles;
  update_text_layers();
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

/**
 * Inizializza i text layer per la stampa di vasche, stile, totale
 */
void init_text_layers(){
  
  // investigare sulle dimensioni
  text_layer_counter = text_layer_create(GRect(0,0,144,40));
  
  // investigare sulle dimensioni
  text_layer_style = text_layer_create(GRect(0,40,144,80));

  // investigare sulle dimensioni
  text_layer_total = text_layer_create(GRect(0,80,144,120));

  update_text_layers();
    
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer_counter));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer_style));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer_total));

}

void update_text_layers(){
  
  // counter
  snprintf(counter_string, sizeof(counter_string), "%d", counter);
  text_layer_set_text(text_layer_counter, counter_string);

  // style
  text_layer_set_text(text_layer_style, styles[current_style_index]);

  // total
  snprintf(total_string, sizeof(total_string), "%d", counter * LUNGHEZZA_VASCA);
  text_layer_set_text(text_layer_total, total_string);

}

void init_data(){
  // TBWFM - sistemare con la size dell'array
  number_of_styles = 4;
  current_style_index = 0;
}

// init function
void init(){
  window = window_create();
  
  init_data();
  init_text_layers();
  
  window_set_click_config_provider(window, click_config_provider);
  
  window_stack_push(window, true);
}

// clean-up function
void deinit(){
  window_destroy(window);
  text_layer_destroy(text_layer_counter);
  text_layer_destroy(text_layer_style);
  text_layer_destroy(text_layer_total);
}

int main(){
  //init the app
  init();
  
  // wait for app events
  app_event_loop();
  
  // clean up the app
  deinit();
  
  // terminate normally
  return 0;
}