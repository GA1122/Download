  virtual void SetUpInProcessBrowserTestFixture() {
    ExtensionApiTest::SetUpInProcessBrowserTestFixture();
    ExtensionTtsController::GetInstance()->SetPlatformImpl(
        &mock_platform_impl_);
  }
