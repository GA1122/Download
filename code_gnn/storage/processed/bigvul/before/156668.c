  float GetVisualViewportScale(FrameTreeNode* node) {
    double scale;
    EXPECT_TRUE(ExecuteScriptAndExtractDouble(
        node, "window.domAutomationController.send(visualViewport.scale);",
        &scale));
    return static_cast<float>(scale);
  }
