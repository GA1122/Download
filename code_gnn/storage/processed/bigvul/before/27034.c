static void fixup_size_hints(PluginInstance *plugin)
{
  NPWindow *window = &plugin->window;

  if (window->width == 0 || window->height == 0) {
	if (plugin->width && plugin->height) {
	  window->width = plugin->width;
	  window->height = plugin->height;
	  return;
	}
  }

  if (window->window && (window->width == 0 || window->height == 0)) {
	XWindowAttributes win_attr;
	if (XGetWindowAttributes(x_display, (Window)window->window, &win_attr)) {
	  plugin->width = window->width = win_attr.width;
	  plugin->height = window->height = win_attr.height;
	  return;
	}
  }

  if (window->width == 0 || window->height == 0)
	npw_printf("WARNING: grmpf, despite much effort, I could not determine the actual plugin area size...\n");
}
