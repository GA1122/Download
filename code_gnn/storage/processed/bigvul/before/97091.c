  void JavascriptGo(std::string distance) {
    GURL url("javascript:history.go('" + distance + "')");
    ASSERT_TRUE(tab_->NavigateToURL(url));
  }
