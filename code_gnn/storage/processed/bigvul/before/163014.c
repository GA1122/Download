  void ValidateRendererAllocations(base::Value* dump_json) {
    SCOPED_TRACE("Validating Renderer Allocation");
    base::ProcessId renderer_pid = base::GetProcId(browser()
                                                       ->tab_strip_model()
                                                       ->GetActiveWebContents()
                                                       ->GetMainFrame()
                                                       ->GetProcess()
                                                       ->GetHandle());
    base::Value* heaps_v2 = FindHeapsV2(renderer_pid, dump_json);
    if (GetParam() == switches::kMemlogModeAll ||
        GetParam() == switches::kMemlogModeRendererSampling) {
      ASSERT_TRUE(heaps_v2);

    } else {
      ASSERT_FALSE(heaps_v2)
          << "There should be no heap dump for the renderer.";
    }

    if (GetParam() == switches::kMemlogModeRendererSampling) {
      EXPECT_EQ(1, NumProcessesWithName(dump_json, "Renderer"));
    } else {
      EXPECT_GT(NumProcessesWithName(dump_json, "Renderer"), 0);
    }
  }
