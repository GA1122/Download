  void ExecuteScriptAndCheckNavigation(
      RenderFrameHost* rfh,
      const std::string& javascript,
      ExpectedNavigationStatus expected_navigation_status) {
    const GURL original_url(shell()->web_contents()->GetLastCommittedURL());
    const std::string expected_message;

    DataURLWarningConsoleObserverDelegate console_delegate(
        shell()->web_contents(), expected_navigation_status);
    shell()->web_contents()->SetDelegate(&console_delegate);

    TestNavigationObserver navigation_observer(shell()->web_contents());
    EXPECT_TRUE(ExecuteScript(rfh, javascript));
    console_delegate.Wait();
    EXPECT_FALSE(console_delegate.saw_failure_message());
    shell()->web_contents()->SetDelegate(nullptr);

    switch (expected_navigation_status) {
      case NAVIGATION_ALLOWED:
        navigation_observer.Wait();
        EXPECT_TRUE(shell()->web_contents()->GetLastCommittedURL().SchemeIs(
            url::kDataScheme));
        EXPECT_TRUE(navigation_observer.last_navigation_url().SchemeIs(
            url::kDataScheme));
        EXPECT_TRUE(navigation_observer.last_navigation_succeeded());
        break;

      case NAVIGATION_BLOCKED:
        EXPECT_EQ(original_url, shell()->web_contents()->GetLastCommittedURL());
        EXPECT_FALSE(navigation_observer.last_navigation_succeeded());
        break;

      default:
        NOTREACHED();
    }
  }
