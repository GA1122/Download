  void RunBasicCopyFromBackingStoreTest() {
    SET_UP_SURFACE_OR_PASS_TEST(NULL);

    int count_attempts = 0;
    while (true) {
      ++count_attempts;
      base::RunLoop run_loop;
      GetRenderViewHost()->CopyFromBackingStore(
          gfx::Rect(),
          frame_size(),
          base::Bind(
              &RenderWidgetHostViewBrowserTest::FinishCopyFromBackingStore,
              base::Unretained(this),
              run_loop.QuitClosure()));
      run_loop.Run();

      if (frames_captured())
        break;
      else
        GiveItSomeTime();
    }

    EXPECT_EQ(count_attempts, callback_invoke_count());
    EXPECT_EQ(1, frames_captured());
  }
