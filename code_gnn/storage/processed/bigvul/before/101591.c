void Browser::CancelWindowClose() {
  DCHECK(is_attempting_to_close_browser_);
  tabs_needing_before_unload_fired_.clear();
  tabs_needing_unload_fired_.clear();
  is_attempting_to_close_browser_ = false;

  TabCloseableStateWatcher* watcher =
      g_browser_process->tab_closeable_state_watcher();
  if (watcher)
    watcher->OnWindowCloseCanceled(this);
}
