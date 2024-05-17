  void AddUnloadHandler(RenderFrameHostImpl* rfh, const std::string& script) {
    EXPECT_FALSE(rfh->GetSuddenTerminationDisablerState(blink::kUnloadHandler));
    EXPECT_TRUE(ExecuteScript(
        rfh, base::StringPrintf("window.onunload = function(e) { %s }",
                                script.c_str())));
    EXPECT_TRUE(rfh->GetSuddenTerminationDisablerState(blink::kUnloadHandler));
  }
