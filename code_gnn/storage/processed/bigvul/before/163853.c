void ExtensionBrowserTest::TearDownOnMainThread() {
  ExtensionMessageBubbleFactory::set_override_for_tests(
      ExtensionMessageBubbleFactory::NO_OVERRIDE);
  extensions::SetExtensionProtocolTestHandler(nullptr);
}
