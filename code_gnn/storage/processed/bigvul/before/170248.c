  void NavigateToTestPage() {
    GURL origin = embedded_test_server()->GetURL(kAutoplayTestPageURL);
    ui_test_utils::NavigateToURL(browser(), origin);

    GURL origin2 = embedded_test_server2()->GetURL(kAutoplayTestPageURL);
    std::string script = base::StringPrintf(
        "setTimeout(\""
        "document.getElementById('subframe').src='%s';"
        "\",0)",
        origin2.spec().c_str());
    content::TestNavigationObserver load_observer(GetWebContents());
    EXPECT_TRUE(ExecuteScriptWithoutUserGesture(GetWebContents(), script));
    load_observer.Wait();
  }
