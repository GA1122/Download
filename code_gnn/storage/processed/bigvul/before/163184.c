  void TestWindowOpenFromFrame(
      const std::string& javascript,
      ExpectedNavigationStatus expected_navigation_status) {
    RenderFrameHost* child =
        ChildFrameAt(shell()->web_contents()->GetMainFrame(), 0);
    if (AreAllSitesIsolatedForTesting()) {
      ASSERT_TRUE(child->IsCrossProcessSubframe());
    }
    ExecuteScriptAndCheckWindowOpen(child, javascript,
                                    expected_navigation_status);
  }
