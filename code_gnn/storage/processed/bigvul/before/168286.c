int BrowserView::GetRenderViewHeightInsetWithDetachedBookmarkBar() {
  if (browser_->bookmark_bar_state() != BookmarkBar::DETACHED ||
      !bookmark_bar_view_ || !bookmark_bar_view_->IsDetached()) {
    return 0;
  }
  return GetLayoutConstant(BOOKMARK_BAR_NTP_HEIGHT);
}
