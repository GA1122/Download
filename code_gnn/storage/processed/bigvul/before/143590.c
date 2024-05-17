  Page* DetectOnceOnBlankPage() {
    WebViewImpl* web_view = web_view_helper_.InitializeAndLoad("about:blank");
    Page* page = web_view->MainFrameImpl()->GetFrame()->GetPage();
    EXPECT_FALSE(page->Paused());
    RunDetection(true, false);
    return page;
  }
