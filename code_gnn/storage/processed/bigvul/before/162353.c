  void PrepareDelegateForCreation(
      std::unique_ptr<AudioInputDelegate> delegate) {
    ASSERT_EQ(nullptr, delegate_);
    delegate_.swap(delegate);
  }
