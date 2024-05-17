  std::unique_ptr<AudioOutputDelegate> CreateDelegate(
      AudioOutputDelegate::EventHandler* handler) {
    MockCreateDelegate(handler);
    EXPECT_NE(nullptr, delegate_);
    return std::move(delegate_);
  }
