void TabCloseableStateWatcher::TabStripWatcher::TabDetachedAt(
    TabContentsWrapper* tab_contents, int index) {
  main_watcher_->OnTabStripChanged(browser_, false);
}
