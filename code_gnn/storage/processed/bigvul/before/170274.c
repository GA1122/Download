  bool TryAutoplay(content::RenderFrameHost* rfh) {
    bool result = false;

    EXPECT_TRUE(content::ExecuteScriptWithoutUserGestureAndExtractBool(
        rfh, "tryPlayback();", &result));

    return result;
  }
