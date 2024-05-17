void TabCloseableStateWatcher::OnTabStripChanged(const Browser* browser,
    bool closing_last_tab) {
  if (waiting_for_browser_)
    return;

  if (!closing_last_tab) {
    CheckAndUpdateState(browser);
    return;
  }

  BrowserActionType action_type;
  CanCloseBrowserImpl(browser, &action_type);
  if (action_type != NONE) {
    Browser* mutable_browser = const_cast<Browser*>(browser);
    if (action_type == OPEN_WINDOW)
      mutable_browser->NewWindow();
    else if (action_type == OPEN_NTP)
      mutable_browser->NewTab();
  }
}
