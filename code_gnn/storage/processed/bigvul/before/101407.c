  void SyncNotifierAddObserver(
      sync_notifier::SyncNotifierObserver* sync_notifier_observer) {
    EXPECT_EQ(NULL, sync_notifier_observer_);
    sync_notifier_observer_ = sync_notifier_observer;
  }
