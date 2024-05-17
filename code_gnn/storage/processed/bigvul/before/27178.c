static int update_window(PluginInstance *plugin, NPWindow *window)
{
  if (plugin->is_windowless) {
	npw_printf("ERROR: update_window() called for windowless plugin\n");
	return -1;
  }

  if (window->ws_info == NULL) {
	npw_printf("ERROR: no window attributes for window %p\n", window->window);
	return -1;
  }

  NPSetWindowCallbackStruct *ws_info = plugin->window.ws_info;
  memcpy(ws_info, window->ws_info, sizeof(*ws_info));
  create_window_attributes(ws_info);

  memcpy(&plugin->window.clipRect, &window->clipRect, sizeof(window->clipRect));;

  if (plugin->window.x != window->x || plugin->window.y != window->y) {
	plugin->window.x = window->x;
	plugin->window.y = window->y;
  }

  if (plugin->window.width != window->width || plugin->window.height != window->height) {
	plugin->window.width = window->width;
	plugin->window.height = window->height;
	if (plugin->toolkit_data) {
	  if (plugin->use_xembed) {
	  }
	  else {
		XtData *toolkit = (XtData *)plugin->toolkit_data;
		if (toolkit->form)
		  XtResizeWidget(toolkit->form, plugin->window.width, plugin->window.height, 0);
		if (toolkit->top_widget)
		  XtResizeWidget(toolkit->top_widget, plugin->window.width, plugin->window.height, 0);
	  }
	}
  }
  return 0;
}