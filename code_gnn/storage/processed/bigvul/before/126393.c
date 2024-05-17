gboolean BrowserWindowGtk::OnBookmarkBarExpose(GtkWidget* sender,
                                               GdkEventExpose* expose) {
  if (browser_->bookmark_bar_state() == BookmarkBar::DETACHED)
    return FALSE;

  return OnExposeDrawInfobarBits(sender, expose);
}
