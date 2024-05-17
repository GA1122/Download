void BrowserWindowGtk::BookmarkBarStateChanged(
    BookmarkBar::AnimateChangeType change_type) {
  MaybeShowBookmarkBar(change_type == BookmarkBar::ANIMATE_STATE_CHANGE);
}
