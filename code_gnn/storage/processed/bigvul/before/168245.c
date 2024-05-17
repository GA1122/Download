void BrowserView::EnterFullscreen(const GURL& url,
                                  ExclusiveAccessBubbleType bubble_type) {
  if (IsFullscreen())
    return;   

  ProcessFullscreen(true, url, bubble_type);
}
