bool TabCloseableStateWatcher::CanCloseTab(const Browser* browser) const {
  return browser->type() != Browser::TYPE_NORMAL ? true :
       (can_close_tab_ || waiting_for_browser_);
 }
