WebPluginProxy::WebPluginProxy(
    PluginChannel* channel,
    int route_id,
    const GURL& page_url,
    gfx::NativeViewId containing_window,
    int host_render_view_routing_id)
    : channel_(channel),
      route_id_(route_id),
      window_npobject_(NULL),
      plugin_element_(NULL),
      delegate_(NULL),
      waiting_for_paint_(false),
      containing_window_(containing_window),
      page_url_(page_url),
      transparent_(false),
      windowless_buffer_index_(0),
      host_render_view_routing_id_(host_render_view_routing_id),
      ALLOW_THIS_IN_INITIALIZER_LIST(weak_factory_(this)) {
#if defined(USE_X11)
  windowless_shm_pixmaps_[0] = None;
  windowless_shm_pixmaps_[1] = None;
  use_shm_pixmap_ = false;

  Display* display = ui::GetXDisplay();
  if (ui::QuerySharedMemorySupport(display) == ui::SHARED_MEMORY_PIXMAP &&
      ui::BitsPerPixelForPixmapDepth(
          display, DefaultDepth(display, DefaultScreen(display))) == 32) {
    Visual* vis = DefaultVisual(display, DefaultScreen(display));

    if (vis->red_mask == 0xff0000 &&
        vis->green_mask == 0xff00 &&
        vis->blue_mask == 0xff)
      use_shm_pixmap_ = true;
  }
#endif
}
