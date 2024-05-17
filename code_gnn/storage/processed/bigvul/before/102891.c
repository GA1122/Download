bool TabCloseableStateWatcher::CanCloseBrowserImpl(
    const Browser* browser,
    BrowserActionType* action_type) {
  *action_type = NONE;

  if (waiting_for_browser_)
    return true;

  if (signing_off_)
    return true;

  if (browser->type() != Browser::TYPE_NORMAL)
    return true;

  if (tabstrip_watchers_.size() > 1)
    return true;

  if (browser->profile()->IsOffTheRecord() && !guest_session_) {
    *action_type = OPEN_WINDOW;
    waiting_for_browser_ = true;
    return true;
  }

  if (!can_close_tab_)
    return false;

  *action_type = OPEN_NTP;
  return false;
}
