  std::string GetURL(const std::string& relative_url) const {
    return embedded_test_server()->GetURL(relative_url).spec();
  }
