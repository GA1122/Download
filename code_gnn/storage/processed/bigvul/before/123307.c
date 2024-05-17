void RenderWidgetHostViewGtk::InitAsFullscreen(
    RenderWidgetHostView* reference_host_view) {
  DCHECK(reference_host_view);
  DoSharedInit();

  is_fullscreen_ = true;
  GtkWindow* window = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));
  gtk_window_set_decorated(window, FALSE);
  destroy_handler_id_ = g_signal_connect(GTK_WIDGET(window),
                                         "destroy",
                                         G_CALLBACK(OnDestroyThunk),
                                         this);
  gtk_container_add(GTK_CONTAINER(window), view_.get());

  GdkScreen* screen = gtk_window_get_screen(window);
  GdkWindow* ref_gdk_window = gtk_widget_get_window(
      reference_host_view->GetNativeView());

  gfx::Rect bounds;
  if (ref_gdk_window) {
    const int monitor_id = gdk_screen_get_monitor_at_window(screen,
                                                            ref_gdk_window);
    GdkRectangle monitor_rect;
    gdk_screen_get_monitor_geometry(screen, monitor_id, &monitor_rect);
    bounds = gfx::Rect(monitor_rect);
  } else {
    bounds = gfx::Rect(
        0, 0, gdk_screen_get_width(screen), gdk_screen_get_height(screen));
  }
  gtk_window_move(window, bounds.x(), bounds.y());
  gtk_window_resize(window, bounds.width(), bounds.height());
  gtk_window_fullscreen(window);
  DoPopupOrFullscreenInit(window, bounds);
}
