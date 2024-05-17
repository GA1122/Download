void BrowserView::UpdateFullscreenExitBubbleContent(
    const GURL& url,
    FullscreenExitBubbleType bubble_type) {
  if (bubble_type == FEB_TYPE_NONE) {
    fullscreen_bubble_.reset();
  } else if (fullscreen_bubble_.get()) {
    fullscreen_bubble_->UpdateContent(url, bubble_type);
  } else {
    fullscreen_bubble_.reset(new FullscreenExitBubbleViews(
        GetWidget(), browser_.get(), url, bubble_type));
  }
}
