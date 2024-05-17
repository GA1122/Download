void BrowserWindowGtk::EnterFullscreen(
      const GURL& url, FullscreenExitBubbleType type) {
  gtk_window_fullscreen(window_);
  fullscreen_exit_bubble_type_ = type;
}
