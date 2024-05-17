void BrowserWindowGtk::InvalidateInfoBarBits() {
  gtk_widget_queue_draw(toolbar_border_);
  gtk_widget_queue_draw(toolbar_->widget());
  if (bookmark_bar_.get() &&
      browser_->bookmark_bar_state() != BookmarkBar::DETACHED) {
    gtk_widget_queue_draw(bookmark_bar_->widget());
  }
}
