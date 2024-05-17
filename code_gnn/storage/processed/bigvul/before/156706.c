  void SetUpEmbeddedTestServer() {
    SetupCrossSiteRedirector(test_server_.get());
    test_server_->RegisterRequestHandler(base::Bind(
        &RequestDelayingSitePerProcessBrowserTest::HandleMockResource,
        base::Unretained(this)));
    ASSERT_TRUE(test_server_->Start());
  }
