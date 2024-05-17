  void NavigateToURL(const std::string& dest_html_file) {
    GURL dest_url = test_server()->GetURL(dest_html_file);
    NavigateToURLImpl(dest_url);
  }
