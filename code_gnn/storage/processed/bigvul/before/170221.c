  static GURL GetExpectedSearchURL(bool expect_safe_search) {
    std::string expected_url("http://google.com/");
    if (expect_safe_search) {
      expected_url += "?" +
                      std::string(safe_search_util::kSafeSearchSafeParameter) +
                      "&" + safe_search_util::kSafeSearchSsuiParameter;
    }
    return GURL(expected_url);
  }
