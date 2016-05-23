#include <pebble.h>

// Functions
void update_text_layers();
void init_text_layers();

static void select_click_handler(ClickRecognizerRef recognizer, void *context);
static void up_click_handler(ClickRecognizerRef recognizer, void *context);
static void down_click_handler(ClickRecognizerRef recognizer, void *context);
static void click_config_provider(void *context);

const int LUNGHEZZA_VASCA = 2 * 25;
Window *window;
TextLayer *text_layer_counter;
TextLayer *text_layer_style;
TextLayer *text_layer_total;
TextLayer *text_layer_detail;
char detail_string[512];

// contatore delle vasche
int counter;
// TBWFM - andrebbe allocata dinamicamente
char counter_string[4];

// distanza percorsa totale
int distanza_totale;
// TBWFM - andrebbe allocata dinamicamente
char distanza_totale_string[10];

// indicazione dello stile
int current_style_index;
const int number_of_styles = 5;
const char *styles [] = {" Rana", " Stile Libero", " Dorso", " Gambe Dorso", " Braccia Stile"};
int distanza_per_stile[5];

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  //text_layer_set_text(text_layer, "Select");
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(counter >= 0){
    distanza_totale += LUNGHEZZA_VASCA;
    distanza_per_stile[current_style_index] += LUNGHEZZA_VASCA;
  }
  counter += 1;
  update_text_layers();
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  
  counter = -1;
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
  
  int y1 = 0;
  int y2 = 42;
  text_layer_counter = text_layer_create(GRect(0, y1, 144, y2));
  text_layer_set_text_alignment(text_layer_counter, GTextAlignmentCenter);
  text_layer_set_font(text_layer_counter, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  
  y1 = y2 + 1;
  y2 = y1 + 28;
  text_layer_style = text_layer_create(GRect(0, y1, 144, y2)); 
  text_layer_set_text_alignment(text_layer_style, GTextAlignmentLeft);
  text_layer_set_font(text_layer_style, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));

  y1 = y2 + 1;
  y2 = y1 + 28;
  text_layer_total = text_layer_create(GRect(0, y1, 144, y2));
  text_layer_set_text_alignment(text_layer_total, GTextAlignmentCenter);
  text_layer_set_font(text_layer_total, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));

  y1 = y2 + 1;
  y2 = 168;
  text_layer_detail = text_layer_create(GRect(0, y1, 144, y2));
  text_layer_set_text_alignment(text_layer_detail, GTextAlignmentLeft);
  text_layer_set_font(text_layer_detail, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));

  update_text_layers();
    
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer_counter));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer_style));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer_total));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer_detail));

}

void update_text_layers(){
  
  // counter
  if(counter == -1){
    snprintf(counter_string, sizeof(counter_string), "%s", "-");
    text_layer_set_text(text_layer_counter, counter_string);
  } else {
    snprintf(counter_string, sizeof(counter_string), "%d", counter);
    text_layer_set_text(text_layer_counter, counter_string);
  }
  
  // style
  text_layer_set_text(text_layer_style, styles[current_style_index]);

  // total
  snprintf(distanza_totale_string, sizeof(distanza_totale_string), "%d", distanza_totale);
  text_layer_set_text(text_layer_total, distanza_totale_string);
  
  // detail
  snprintf(detail_string, sizeof(detail_string), " Rana: %d - Stile Libero: %d - Dorso: %d - Gambe Dorso: %d - Braccia Stile %d ", distanza_per_stile[0], distanza_per_stile[1], distanza_per_stile[2], distanza_per_stile[3], distanza_per_stile[4]);
  text_layer_set_text(text_layer_detail, detail_string);
}

void init_data(){
  int i;
  
  counter = -1;
  distanza_totale = 0;
  current_style_index = 0;
  
  for(i=0; i<number_of_styles; i++ ){
    distanza_per_stile[i] = 0;
  }
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