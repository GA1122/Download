TabCloseableStateWatcher::TabStripWatcher::TabStripWatcher(
    TabCloseableStateWatcher* main_watcher, const Browser* browser)
    : main_watcher_(main_watcher),
      browser_(browser) {
  browser_->tabstrip_model()->AddObserver(this);
}
