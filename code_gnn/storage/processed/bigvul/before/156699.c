  explicit SameDocumentCommitObserver(WebContents* web_contents)
      : WebContentsObserver(web_contents) {
    EXPECT_TRUE(web_contents);
  }
