  void TestFinishedUnwound() {
    TearDownTest();
    test_finished_event_->Signal();
  }
