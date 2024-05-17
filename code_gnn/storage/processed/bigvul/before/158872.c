void Browser::UpdateBookmarkBarState(BookmarkBarStateChangeReason reason) {
  BookmarkBar::State state;
  if (profile_->IsGuestSession()) {
    state = BookmarkBar::HIDDEN;
  } else if (browser_defaults::bookmarks_enabled &&
      profile_->GetPrefs()->GetBoolean(bookmarks::prefs::kShowBookmarkBar) &&
      !ShouldHideUIForFullscreen()) {
    state = BookmarkBar::SHOW;
  } else {
    WebContents* web_contents = tab_strip_model_->GetActiveWebContents();
    BookmarkTabHelper* bookmark_tab_helper =
        web_contents ? BookmarkTabHelper::FromWebContents(web_contents) : NULL;
    if (bookmark_tab_helper && bookmark_tab_helper->ShouldShowBookmarkBar())
      state = BookmarkBar::DETACHED;
    else
      state = BookmarkBar::HIDDEN;
  }

  if (state == bookmark_bar_state_)
    return;

  bookmark_bar_state_ = state;

  if (!window_)
    return;   

  if (reason == BOOKMARK_BAR_STATE_CHANGE_TAB_SWITCH) {
    return;
  }

  bool should_animate = reason == BOOKMARK_BAR_STATE_CHANGE_PREF_CHANGE;
  window_->BookmarkBarStateChanged(should_animate ?
      BookmarkBar::ANIMATE_STATE_CHANGE :
      BookmarkBar::DONT_ANIMATE_STATE_CHANGE);
}
