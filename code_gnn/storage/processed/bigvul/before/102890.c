 bool TabCloseableStateWatcher::CanCloseBrowser(Browser* browser) {
   BrowserActionType action_type;
   bool can_close = CanCloseBrowserImpl(browser, &action_type);
  if (action_type == OPEN_WINDOW) {
    browser->NewWindow();
  } else if (action_type == OPEN_NTP) {
    browser->CloseAllTabs();
  }
  return can_close;
}
