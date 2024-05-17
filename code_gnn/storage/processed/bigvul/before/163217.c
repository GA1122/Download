  WebContentsLoadFinishedWaiter(WebContents* web_contents,
                                const GURL& expected_url)
      : WebContentsObserver(web_contents),
        expected_url_(expected_url),
        message_loop_runner_(new MessageLoopRunner) {
    EXPECT_TRUE(web_contents != NULL);
  }
