  void TestTransitionFromActiveToFrozen() {
    TestTransitionFromActiveToPendingFreeze();

    {
      ExpectStateTransitionObserver expect_state_transition(
          GetLifecycleUnitAt(1), LifecycleUnitState::FROZEN);
      expect_state_transition.Wait();
    }

    content::WebContents* const content = GetWebContentsAt(1);
    content::RenderFrameHost* main_frame = content->GetMainFrame();
    content::RenderFrameHost* child_frame = content->GetAllFrames()[1];

    int freeze_count_result = 0;
    EXPECT_TRUE(content::ExecuteScriptAndExtractInt(
        main_frame, kMainFrameFrozenStateJS, &freeze_count_result));
    EXPECT_EQ(1, freeze_count_result);
    EXPECT_TRUE(content::ExecuteScriptAndExtractInt(
        child_frame, kChildFrameFrozenStateJS, &freeze_count_result));
    EXPECT_EQ(1, freeze_count_result);
  }
