  void SyncNotifierRemoveObserver(
      sync_notifier::SyncNotifierObserver* sync_notifier_observer) {
    EXPECT_EQ(sync_notifier_observer_, sync_notifier_observer);
    sync_notifier_observer_ = NULL;
  }
