  void AddIFrame(RenderFrameHost* rfh, const GURL& url) {
    const std::string javascript = base::StringPrintf(
        "f = document.createElement('iframe'); f.src = '%s';"
        "document.body.appendChild(f);",
        url.spec().c_str());
    TestNavigationObserver observer(shell()->web_contents());
    EXPECT_TRUE(ExecuteScript(rfh, javascript));
    observer.Wait();
  }
