  void ForceTimeout() {
    static_cast<TestOneShotGestureSequenceTimer*>(
        GetLongPressTimer())->ForceTimeout();
  }
