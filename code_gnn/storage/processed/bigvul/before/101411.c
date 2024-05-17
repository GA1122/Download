  void TearDown() {
    sync_manager_.RemoveObserver(&observer_);
    sync_manager_.Shutdown();
    sync_notifier_mock_ = NULL;
    EXPECT_FALSE(sync_notifier_observer_);
    PumpLoop();
  }
