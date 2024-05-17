  LoadFinishedWaiter(WebContents* web_contents, const GURL& expected_url)
      : WebContentsObserver(web_contents),
        expected_url_(expected_url),
        run_loop_(new base::RunLoop()) {
    EXPECT_TRUE(web_contents != nullptr);
  }
