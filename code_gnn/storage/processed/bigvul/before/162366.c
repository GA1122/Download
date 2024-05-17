  void PrepareDelegateForCreation(
      std::unique_ptr<AudioOutputDelegate> delegate) {
    ASSERT_EQ(nullptr, delegate_);
    delegate_.swap(delegate);
  }
