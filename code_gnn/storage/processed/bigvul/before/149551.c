std::string GetPathWithPortReplacement(const std::string& path, uint16_t port) {
  std::string string_port = base::StringPrintf("%d", port);
  return net::test_server::GetFilePathWithReplacements(
      path, {{"REPLACE_WITH_PORT", string_port}});
}
