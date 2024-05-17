bool ExtensionBrowserTest::ExecuteScriptInBackgroundPageNoWait(
    const std::string& extension_id,
    const std::string& script) {
  return extensions::browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      profile(), extension_id, script);
}
