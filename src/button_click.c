#include <pebble.h>

static Window *window;
static TextLayer *text_layer;

#define DEFAULT_COUNT 0
#define COUNT_KEY 43

int count;
char text_count[10];

static void p_count(int count){
  snprintf(text_count, 10, "%05d", count);
  text_layer_set_text(text_layer, text_count);
}


static void set_persist(){
  persist_write_int(COUNT_KEY, 14);
}
static void check_persist(){
  // if (persist_exists(COUNT_KEY)){
  //   text_layer_set_text(text_layer, "EXIST");
  // }else{
  //   text_layer_set_text(text_layer, "NOT EXIST");
  // }
  p_count(persist_read_int(COUNT_KEY));
}
static void delete_persist(){
  persist_delete(COUNT_KEY);
}


static void select_click_handler(){
  // count = DEFAULT_COUNT;
  persist_write_int(COUNT_KEY, count);
  p_count(count);
}

static void up_click_handler() {
  count++;
  p_count(count);
}

static void down_click_handler() {
  count--;
  p_count(count);
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, check_persist);
  // window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, set_persist);
  // window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, delete_persist);
  // window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  p_count(count);
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}

static void init(void) {
  count = persist_exists(COUNT_KEY) ? persist_read_int(COUNT_KEY) : DEFAULT_COUNT;
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  persist_write_int(COUNT_KEY, count);
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
  return 0;
}
