void SyncManager::TriggerOnNotificationStateChangeForTest(
    bool notifications_enabled) {
  DCHECK(thread_checker_.CalledOnValidThread());
  data_->OnNotificationStateChange(notifications_enabled);
}
