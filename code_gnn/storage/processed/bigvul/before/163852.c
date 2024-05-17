void ExtensionBrowserTest::SetUpOnMainThread() {
  observer_.reset(
      new extensions::ChromeExtensionTestNotificationObserver(browser()));
  if (extension_service()->updater()) {
    extension_service()->updater()->SetExtensionCacheForTesting(
        test_extension_cache_.get());
  }

  base::FilePath test_root_path;
  PathService::Get(chrome::DIR_TEST_DATA, &test_root_path);
  test_root_path = test_root_path.AppendASCII("extensions");
  test_protocol_handler_ =
      base::Bind(&ExtensionProtocolTestHandler, test_root_path);
  extensions::SetExtensionProtocolTestHandler(&test_protocol_handler_);
}
