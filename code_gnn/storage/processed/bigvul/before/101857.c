void Browser::UpdateBookmarkBarState(BookmarkBarStateChangeReason reason) {
  BookmarkBar::State state;
  if ((profile_->GetPrefs()->GetBoolean(prefs::kShowBookmarkBar) &&
       profile_->GetPrefs()->GetBoolean(prefs::kEnableBookmarkBar)) &&
      (!window_ || !window_->IsFullscreen())) {
    state = BookmarkBar::SHOW;
  } else {
    TabContentsWrapper* tab = GetSelectedTabContentsWrapper();
    if (tab && tab->bookmark_tab_helper()->ShouldShowBookmarkBar())
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

  BookmarkBar::AnimateChangeType animate_type =
      (reason == BOOKMARK_BAR_STATE_CHANGE_PREF_CHANGE) ?
      BookmarkBar::ANIMATE_STATE_CHANGE :
      BookmarkBar::DONT_ANIMATE_STATE_CHANGE;
  window_->BookmarkBarStateChanged(animate_type);
}
