void TabCloseableStateWatcher::TabStripWatcher::TabInsertedAt(
    TabContentsWrapper* tab_contents, int index, bool foreground) {
  main_watcher_->OnTabStripChanged(browser_, false);
}
