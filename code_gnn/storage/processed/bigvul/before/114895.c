void TestingAutomationProvider::GetBookmarkBarVisibility(int handle,
                                                         bool* visible,
                                                         bool* animating,
                                                         bool* detached) {
  *visible = false;
  *animating = false;

  if (browser_tracker_->ContainsHandle(handle)) {
    Browser* browser = browser_tracker_->GetResource(handle);
    if (browser) {
      *visible = browser->bookmark_bar_state() == BookmarkBar::SHOW;
      *animating = browser->window()->IsBookmarkBarAnimating();
      *detached = browser->bookmark_bar_state() == BookmarkBar::DETACHED;
    }
  }
}
