  std::unique_ptr<FakeMediaStreamUIProxy> CreateFakeUI() {
    return std::make_unique<FakeMediaStreamUIProxy>(
         true);
  }
