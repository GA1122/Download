void ExtensionTabsZoomTest::SetUpOnMainThread() {
  ExtensionTabsTest::SetUpOnMainThread();
  extension_ = ExtensionBuilder("Test").Build();
}
