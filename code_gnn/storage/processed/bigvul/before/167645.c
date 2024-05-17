  std::string GetInputValue(RenderFrameHostImpl* frame) {
    std::string result;
    EXPECT_TRUE(ExecuteScriptAndExtractString(
        frame, "window.domAutomationController.send(input.value);", &result));
    return result;
  }
