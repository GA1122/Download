void Browser::ModeChanged(const chrome::search::Mode& old_mode,
                          const chrome::search::Mode& new_mode) {
  UpdateBookmarkBarState(BOOKMARK_BAR_STATE_CHANGE_TAB_STATE);
}
