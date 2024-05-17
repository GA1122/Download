void BrowserView::ShowBookmarkBubble(const GURL& url, bool already_bookmarked) {
  chrome::ShowBookmarkBubbleView(GetToolbarView()->GetBookmarkBubbleAnchor(),
                                 browser_->profile(), url, !already_bookmarked);
}
