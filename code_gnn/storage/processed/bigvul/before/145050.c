std::string NormalizeLayoutTestURL(const std::string& url) {
  std::string result = url;
  size_t pos;
  if (!url.find(file_url_pattern) &&
      ((pos = url.find(layout_tests_pattern)) != std::string::npos)) {
    result.replace(0, pos + layout_tests_pattern_size, file_test_prefix);
  } else if (!url.find(data_url_pattern)) {
    std::string path = url.substr(data_url_pattern_size);
    result.replace(data_url_pattern_size, url.length(), path);
  }
  return result;
}
