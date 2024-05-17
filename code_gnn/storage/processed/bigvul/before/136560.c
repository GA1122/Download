  void TestSubsequenceBecomesEmpty() {
    FakeDisplayItemClient target("target");
    GraphicsContext context(GetPaintController());

    InitRootChunk();
    {
      SubsequenceRecorder r(context, target);
      DrawRect(context, target, kBackgroundType, FloatRect(100, 100, 300, 300));
    }
    GetPaintController().CommitNewDisplayItems();

    InitRootChunk();
    {
      EXPECT_FALSE(
          SubsequenceRecorder::UseCachedSubsequenceIfPossible(context, target));
      SubsequenceRecorder r(context, target);
    }
    GetPaintController().CommitNewDisplayItems();
  }
