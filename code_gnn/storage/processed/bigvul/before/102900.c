void TabCloseableStateWatcher::TabStripWatcher::TabChangedAt(
    TabContentsWrapper* tab_contents, int index, TabChangeType change_type) {
  main_watcher_->OnTabStripChanged(browser_, false);
}
