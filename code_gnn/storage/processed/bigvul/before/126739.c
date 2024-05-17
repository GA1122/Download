void BrowserView::EnterFullscreen(
    const GURL& url, FullscreenExitBubbleType bubble_type) {
  if (IsFullscreen())
    return;   

  ProcessFullscreen(true, FOR_DESKTOP, url, bubble_type);
}
