void TabCloseableStateWatcher::TabStripWatcher::TabClosingAt(
    TabStripModel* tab_strip_model,
    TabContentsWrapper* tab_contents,
    int index) {
  if (tab_strip_model->count() == 1)
    main_watcher_->OnTabStripChanged(browser_, true);
}
