  virtual void SetUp() {
    RenderViewHostTestHarness::SetUp();
    test_personal_data_ = new TestPersonalDataManager();
    autofill_manager_.reset(new TestAutoFillManager(contents(),
                                                    test_personal_data_.get()));
  }
