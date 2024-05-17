bool RenderWidgetHostViewGtk::LockMouse() {
  if (mouse_locked_)
    return true;

  mouse_locked_ = true;

  GtkWidget* current_grab_window = gtk_grab_get_current();
  if (current_grab_window) {
    gtk_grab_remove(current_grab_window);
    LOG(WARNING) << "Locking Mouse with gdk_pointer_grab, "
                 << "but had to steal grab from another window";
  }

  GtkWidget* widget = view_.get();
  GdkWindow* window = gtk_widget_get_window(widget);
  GdkCursor* cursor = gdk_cursor_new(GDK_BLANK_CURSOR);

  GdkGrabStatus grab_status =
      gdk_pointer_grab(window,
                       FALSE,   
                       static_cast<GdkEventMask>(
                           GDK_POINTER_MOTION_MASK |
                           GDK_BUTTON_PRESS_MASK |
                           GDK_BUTTON_RELEASE_MASK),
                       window,   
                       cursor,
                       GDK_CURRENT_TIME);

  if (grab_status != GDK_GRAB_SUCCESS) {
    LOG(WARNING) << "Failed to grab pointer for LockMouse. "
                 << "gdk_pointer_grab returned: " << grab_status;
    mouse_locked_ = false;
    return false;
  }

  SetTooltipText(string16());

  MarkCachedWidgetCenterStale();

  return true;
}
