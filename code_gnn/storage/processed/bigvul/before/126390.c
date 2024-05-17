void BrowserWindowGtk::MaybeShowBookmarkBar(bool animate) {
  TRACE_EVENT0("ui::gtk", "BrowserWindowGtk::MaybeShowBookmarkBar");
  if (!IsBookmarkBarSupported())
    return;

  TabContents* tab = GetDisplayedTab();

  if (tab)
    bookmark_bar_->SetPageNavigator(browser_.get());

  BookmarkBar::State state = browser_->bookmark_bar_state();
  if (contents_container_->HasPreview() && state == BookmarkBar::DETACHED)
    state = BookmarkBar::HIDDEN;

  toolbar_->UpdateForBookmarkBarVisibility(state == BookmarkBar::DETACHED);
  PlaceBookmarkBar(state == BookmarkBar::DETACHED);
  bookmark_bar_->SetBookmarkBarState(
      state,
      animate ? BookmarkBar::ANIMATE_STATE_CHANGE :
                BookmarkBar::DONT_ANIMATE_STATE_CHANGE);
}
