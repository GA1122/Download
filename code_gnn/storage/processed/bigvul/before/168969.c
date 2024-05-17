  void NavigateToURL(const net::EmbeddedTestServer& test_server,
                     const std::string& hostname,
                     const std::string& relative_url) {
    NavigationObserver observer(WebContents());
    GURL url = test_server.GetURL(hostname, relative_url);
    ui_test_utils::NavigateToURL(browser(), url);
    observer.Wait();
  }
