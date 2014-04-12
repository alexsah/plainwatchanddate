#include <pebble.h>

Window *window;
TextLayer *time_layer;
TextLayer *date_layer;
InverterLayer *inverter_layer;

void handle_timechanges(struct tm *tick_time, TimeUnits units_changed) {
    static char time_buffer[6];
    static char date_buffer[25];
    
    strftime(time_buffer, sizeof(time_buffer), "%l:%M", tick_time);
    text_layer_set_text(time_layer, time_buffer);
    
    strftime(date_buffer, sizeof(date_buffer), "%a %b %e", tick_time);
    text_layer_set_text(date_layer, date_buffer);
  
}

void handle_init(void) {
	// Create a window and text layers
	window = window_create();
	time_layer = text_layer_create(GRect(0, 60, 144, 50));
    date_layer = text_layer_create(GRect(0, 0, 144, 50));
	
	// Set the font, and text alignment
    text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49));
	text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  
    text_layer_set_font(date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
	text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
  
	
	// Add the text layer to the window
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer));
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_layer));
    inverter_layer = inverter_layer_create(GRect(0, 0, 144, 168));
	layer_add_child(window_get_root_layer(window), inverter_layer_get_layer(inverter_layer));

    time_t now = time(NULL);
    handle_timechanges(localtime(&now), MINUTE_UNIT);
    tick_timer_service_subscribe(MINUTE_UNIT, handle_timechanges);
  
    // Push the window
	window_stack_push(window, true);
	
	// App Logging!
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed a window!");
}

void handle_deinit(void) {
	// Destroy the text layers
	text_layer_destroy(time_layer);
    text_layer_destroy(date_layer);
	
    //Destroy inverter layer
    inverter_layer_destroy(inverter_layer);
  
	// Destroy the window
	window_destroy(window);
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}
