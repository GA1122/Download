void RenderWidgetHostViewGtk::Destroy() {
  if (compositing_surface_ != gfx::kNullPluginWindow) {
    GtkNativeViewManager* manager = GtkNativeViewManager::GetInstance();
    manager->ReleasePermanentXID(compositing_surface_);
  }

  if (do_x_grab_) {
    GdkDisplay* display = gtk_widget_get_display(parent_);
    gdk_display_pointer_ungrab(display, GDK_CURRENT_TIME);
    gdk_display_keyboard_ungrab(display, GDK_CURRENT_TIME);
  }

  if (IsPopup() || is_fullscreen_) {
    GtkWidget* window = gtk_widget_get_parent(view_.get());

    ui::ActiveWindowWatcherX::RemoveObserver(this);

    if (is_fullscreen_)
      g_signal_handler_disconnect(window, destroy_handler_id_);

    gtk_widget_destroy(window);
  }

  gtk_widget_destroy(view_.get());

  host_ = NULL;

  MessageLoop::current()->DeleteSoon(FROM_HERE, this);
}
