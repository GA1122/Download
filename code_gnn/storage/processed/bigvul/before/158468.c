  void TearDownEnvironment() {
    sink_ = nullptr;
    process_host_ = nullptr;
    if (view_)
      DestroyView(view_);

    parent_view_->Destroy();
    delete parent_host_;

    browser_context_.reset();
    aura_test_helper_->TearDown();

    base::RunLoop().RunUntilIdle();
    ImageTransportFactory::Terminate();
  }
