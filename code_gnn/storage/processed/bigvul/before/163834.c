extensions::ExtensionHost* ExtensionBrowserTest::FindHostWithPath(
    extensions::ProcessManager* manager,
    const std::string& path,
    int expected_hosts) {
  extensions::ExtensionHost* result_host = nullptr;
  int num_hosts = 0;
  for (extensions::ExtensionHost* host : manager->background_hosts()) {
    if (host->GetURL().path() == path) {
      EXPECT_FALSE(result_host);
      result_host = host;
    }
    num_hosts++;
  }
  EXPECT_EQ(expected_hosts, num_hosts);
  return result_host;
}
