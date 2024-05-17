  void ExecuteScriptAndCheckWindowOpen(
      RenderFrameHost* rfh,
      const std::string& javascript,
      ExpectedNavigationStatus expected_navigation_status) {
    ShellAddedObserver new_shell_observer;
    EXPECT_TRUE(ExecuteScript(rfh, javascript));

    Shell* new_shell = new_shell_observer.GetShell();
    WaitForLoadStop(new_shell->web_contents());

    switch (expected_navigation_status) {
      case NAVIGATION_ALLOWED:
        EXPECT_TRUE(new_shell->web_contents()->GetLastCommittedURL().SchemeIs(
            url::kDataScheme));
        break;

      case NAVIGATION_BLOCKED:
        EXPECT_TRUE(
            new_shell->web_contents()->GetLastCommittedURL().is_empty());
        break;

      default:
        NOTREACHED();
    }
  }
