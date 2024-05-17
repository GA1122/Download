void RenderWidgetHostViewGtk::ActiveWindowChanged(GdkWindow* window) {
  GdkWindow* our_window = gtk_widget_get_parent_window(view_.get());

  if (our_window == window)
    made_active_ = true;

  if (is_fullscreen_ && our_window != window && made_active_)
    host_->Shutdown();
}
