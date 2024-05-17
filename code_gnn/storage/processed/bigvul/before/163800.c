  GURL GetTestBaseURL(const std::string& test_directory) {
    GURL::Replacements replace_host;
    replace_host.SetHostStr("localhost");
    GURL base_url = embedded_test_server()->GetURL(
        "/extensions/api_test/" + test_directory + "/");
    return base_url.ReplaceComponents(replace_host);
  }
