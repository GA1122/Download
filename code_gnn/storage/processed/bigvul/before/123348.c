void RenderWidgetHostViewGtk::UnlockMouse() {
  if (!mouse_locked_)
    return;

  mouse_locked_ = false;

  GtkWidget* widget = view_.get();
  GdkDisplay* display = gtk_widget_get_display(widget);
  GdkScreen* screen = gtk_widget_get_screen(widget);
  gdk_display_pointer_ungrab(display, GDK_CURRENT_TIME);
  gdk_display_warp_pointer(display, screen,
                           unlocked_global_mouse_position_.x(),
                           unlocked_global_mouse_position_.y());
  mouse_is_being_warped_to_unlocked_position_ = true;

  if (host_)
    host_->LostMouseLock();
}
