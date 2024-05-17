void BrowserWindowGtk::ShowBookmarkBubble(const GURL& url,
                                          bool already_bookmarked) {
  toolbar_->GetLocationBarView()->ShowStarBubble(url, !already_bookmarked);
}
