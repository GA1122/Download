std::string ExtensionBrowserTest::ExecuteScriptInBackgroundPage(
    const std::string& extension_id,
    const std::string& script) {
  return extensions::browsertest_util::ExecuteScriptInBackgroundPage(
      profile(), extension_id, script);
}
