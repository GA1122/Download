void BrowserWindowGtk::SetBoundsImpl(const gfx::Rect& bounds,
                                     bool exterior,
                                     bool move) {
  gint x = static_cast<gint>(bounds.x());
  gint y = static_cast<gint>(bounds.y());
  gint width = static_cast<gint>(bounds.width());
  gint height = static_cast<gint>(bounds.height());

  if (move)
    gtk_window_move(window_, x, y);

  if (exterior) {
    gtk_window_util::SetWindowSize(window_, gfx::Size(width, height));
  } else {
    gtk_widget_set_size_request(contents_container_->widget(),
                                width, height);
  }
}
