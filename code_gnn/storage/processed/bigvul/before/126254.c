void Browser::UpdateBookmarkBarState(BookmarkBarStateChangeReason reason) {
  BookmarkBar::State state;
  if (browser_defaults::bookmarks_enabled &&
      profile_->GetPrefs()->GetBoolean(prefs::kShowBookmarkBar) &&
      (!window_ || !window_->IsFullscreen())) {
    state = search_model_->mode().is_ntp() ?
        BookmarkBar::DETACHED : BookmarkBar::SHOW;
  } else {
    WebContents* web_contents = chrome::GetActiveWebContents(this);
    BookmarkTabHelper* bookmark_tab_helper =
        web_contents ? BookmarkTabHelper::FromWebContents(web_contents) : NULL;
    if (bookmark_tab_helper && bookmark_tab_helper->ShouldShowBookmarkBar())
      state = BookmarkBar::DETACHED;
    else
      state = BookmarkBar::HIDDEN;
  }

  if (search_model_->mode().is_search_suggestions())
    state = BookmarkBar::HIDDEN;

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
