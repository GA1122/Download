void SyncManager::TriggerOnNotificationStateChangeForTest(
    bool notifications_enabled) {
  data_->OnNotificationStateChange(notifications_enabled);
}
