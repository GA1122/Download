  gfx::Rect GetVisualViewport(FrameTreeNode* node) {
    std::string result;
    EXPECT_TRUE(ExecuteScriptAndExtractString(
        node,
        "window.domAutomationController.send("
        "    rectAsString(visualViewportAsRect()));",
        &result));
    return GetRectFromString(result);
  }
