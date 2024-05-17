void ExtensionApiTest::TearDownOnMainThread() {
  ExtensionBrowserTest::TearDownOnMainThread();
  extensions::TestGetConfigFunction::set_test_config_state(NULL);
  test_config_.reset(NULL);
}
