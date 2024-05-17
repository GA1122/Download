  ActiveTabDelegateTest() {
    auto delegate = std::make_unique<ActiveTabPermissionGranterTestDelegate>();
    test_delegate_ = delegate.get();
    ActiveTabPermissionGranter::SetPlatformDelegate(std::move(delegate));
  }
