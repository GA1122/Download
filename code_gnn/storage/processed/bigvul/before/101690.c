bool Browser::IsClosingPermitted() {
  TabCloseableStateWatcher* watcher =
      g_browser_process->tab_closeable_state_watcher();
  bool can_close = !watcher || watcher->CanCloseBrowser(this);
  if (!can_close && is_attempting_to_close_browser_)
    CancelWindowClose();
  return can_close;
}
