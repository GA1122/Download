  void NavigateToURL(const GURL& url) {
    NavigationCompletedObserver observer(
        browser()->tab_strip_model()->GetActiveWebContents());

    ui_test_utils::NavigateToURL(browser(), url);

    observer.Wait();
  }
