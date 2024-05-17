  void WaitForTouchActionUpdated(
      MainThreadFrameObserver* root_thread_observer,
      MainThreadFrameObserver* child_thread_observer) {
    root_thread_observer->Wait();
    child_thread_observer->Wait();
    root_thread_observer->Wait();
  }
