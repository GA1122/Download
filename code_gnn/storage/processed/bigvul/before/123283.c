void RenderWidgetHostViewGtk::DoPopupOrFullscreenInit(GtkWindow* window,
                                                      const gfx::Rect& bounds) {
  requested_size_.SetSize(std::min(bounds.width(), kMaxWindowWidth),
                          std::min(bounds.height(), kMaxWindowHeight));
  host_->WasResized();

  ui::ActiveWindowWatcherX::AddObserver(this);

  if (!is_fullscreen_) {
    gtk_widget_set_size_request(
        view_.get(), requested_size_.width(), requested_size_.height());

    gtk_window_set_resizable(window, FALSE);
    gtk_window_set_default_size(window, -1, -1);
    gtk_window_move(window, bounds.x(), bounds.y());
  }

  gtk_widget_show_all(GTK_WIDGET(window));
}
