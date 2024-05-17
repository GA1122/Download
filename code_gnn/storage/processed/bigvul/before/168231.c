void BrowserView::BookmarkBarStateChanged(
    BookmarkBar::AnimateChangeType change_type) {
  if (bookmark_bar_view_.get()) {
    BookmarkBar::State new_state = browser_->bookmark_bar_state();

    bool detached_changed = (new_state == BookmarkBar::DETACHED) ||
        bookmark_bar_view_->IsDetached();
    if (detached_changed && immersive_mode_controller_->IsEnabled())
      change_type = BookmarkBar::DONT_ANIMATE_STATE_CHANGE;

    bookmark_bar_view_->SetBookmarkBarState(new_state, change_type);
  }
  if (MaybeShowBookmarkBar(GetActiveWebContents()))
    Layout();
}
