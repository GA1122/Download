  void NavigateToPageWithLinks(Shell* shell) {
    EXPECT_TRUE(NavigateToURL(
        shell, embedded_test_server()->GetURL("/click-noreferrer-links.html")));

    std::string script = "setOriginForLinks('http://bar.com:" +
                         embedded_test_server()->base_url().port() + "/');";
    EXPECT_TRUE(ExecuteScript(shell, script));
  }
