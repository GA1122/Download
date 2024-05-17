  virtual void SetupFakeUI(bool expect_started) {
    media_stream_manager_->UseFakeUIFactoryForTests(
        base::Bind(&MediaStreamDispatcherHostTest::CreateMockUI,
                   base::Unretained(this), expect_started));
  }
