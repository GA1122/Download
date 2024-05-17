void TabCloseableStateWatcher::SetCloseableState(bool closeable) {
  if (can_close_tab_ == closeable)   
    return;

  can_close_tab_ = closeable;

  NotificationService::current()->Notify(
      NotificationType::TAB_CLOSEABLE_STATE_CHANGED,
      NotificationService::AllSources(),
      Details<bool>(&can_close_tab_));
}
