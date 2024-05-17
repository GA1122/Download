   MockDataWriter* NavigateAndReturnMockWriter(const GURL& url) {
    content::WebContentsTester* web_contents_tester =
        content::WebContentsTester::For(web_contents());
    EXPECT_TRUE(web_contents_tester);
    web_contents_tester->NavigateAndCommit(url);
    return static_cast<MockDataWriter*>(observer_->GetWriterForTesting());
  }
