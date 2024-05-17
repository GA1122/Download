void TabCloseableStateWatcher::OnBrowserAdded(const Browser* browser) {
  waiting_for_browser_ = false;

  if (browser->type() != Browser::TYPE_NORMAL)
    return;

  tabstrip_watchers_.push_back(new TabStripWatcher(this, browser));

}
