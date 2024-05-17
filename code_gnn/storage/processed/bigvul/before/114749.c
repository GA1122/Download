void PPAPITestBase::RunTestURL(const GURL& test_url) {
  TestFinishObserver observer(
      browser()->GetSelectedWebContents()->GetRenderViewHost(), kTimeoutMs);

  ui_test_utils::NavigateToURL(browser(), test_url);

  ASSERT_TRUE(observer.WaitForFinish()) << "Test timed out.";

  EXPECT_STREQ("PASS", observer.result().c_str());
}
