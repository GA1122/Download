void ExtensionApiTest::TearDownOnMainThread() {
  ExtensionBrowserTest::TearDownOnMainThread();
  TestGetConfigFunction::set_test_config_state(NULL);
  test_config_.reset(NULL);
}
