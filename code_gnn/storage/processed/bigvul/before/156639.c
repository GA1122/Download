  bool AutoplayAllowed(const ToRenderFrameHost& adapter,
                       bool with_user_gesture) {
    RenderFrameHost* rfh = adapter.render_frame_host();
    const char* test_script = "attemptPlay();";
    bool worked = false;
    if (with_user_gesture) {
      EXPECT_TRUE(ExecuteScriptAndExtractBool(rfh, test_script, &worked));
    } else {
      EXPECT_TRUE(ExecuteScriptWithoutUserGestureAndExtractBool(
          rfh, test_script, &worked));
    }
    return worked;
  }
