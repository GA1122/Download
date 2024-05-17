  std::unique_ptr<AudioInputDelegate> CreateDelegate(
      AudioInputDelegate::EventHandler* handler) {
    MockCreateDelegate(handler);
    EXPECT_NE(nullptr, delegate_);
    return std::move(delegate_);
  }
