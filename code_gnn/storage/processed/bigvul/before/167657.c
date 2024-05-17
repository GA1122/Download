  SameDocumentCommitObserver(WebContents* web_contents)
      : WebContentsObserver(web_contents),
        message_loop_runner_(new MessageLoopRunner) {
    EXPECT_TRUE(web_contents);
  }
