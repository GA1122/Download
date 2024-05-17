bool Browser::CanCloseTab() const {
  TabCloseableStateWatcher* watcher =
      g_browser_process->tab_closeable_state_watcher();
  return !watcher || watcher->CanCloseTab(this);
}
