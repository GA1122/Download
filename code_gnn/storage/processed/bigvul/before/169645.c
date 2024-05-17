  WebContents* OpenBackgroundTab(const GURL& page) {
    ui_test_utils::NavigateToURLWithDisposition(
        browser(), page, WindowOpenDisposition::NEW_BACKGROUND_TAB,
        ui_test_utils::BROWSER_TEST_WAIT_FOR_NAVIGATION);

    TabStripModel* tab_strip = browser()->tab_strip_model();
    WebContents* wc =
        tab_strip->GetWebContentsAt(tab_strip->active_index() + 1);
    CHECK(wc->GetVisibleURL() == page);

    WaitForLauncherThread();
    WaitForMessageProcessing(wc);
    return wc;
  }
