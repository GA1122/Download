  static void OnGrabNotify(GtkWidget* widget, gboolean was_grabbed,
                           RenderWidgetHostViewGtk* host_view) {
    if (was_grabbed) {
      if (host_view->was_imcontext_focused_before_grab_)
        host_view->im_context_->OnFocusIn();
    } else {
      host_view->was_imcontext_focused_before_grab_ =
          host_view->im_context_->is_focused();
      if (host_view->was_imcontext_focused_before_grab_) {
        gdk_window_set_cursor(gtk_widget_get_window(widget), NULL);
        host_view->im_context_->OnFocusOut();
      }
    }
  }
