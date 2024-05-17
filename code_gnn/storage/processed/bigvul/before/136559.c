  void TestMoreDrawingInSubsequence() {
    FakeDisplayItemClient first("first");
    GraphicsContext context(GetPaintController());

    InitRootChunk();
    {
      SubsequenceRecorder r(context, first);
      DrawRect(context, first, kBackgroundType, FloatRect(100, 100, 300, 300));
    }
    GetPaintController().CommitNewDisplayItems();

    InitRootChunk();
    {
      EXPECT_FALSE(
          SubsequenceRecorder::UseCachedSubsequenceIfPossible(context, first));
      SubsequenceRecorder r(context, first);
      DrawRect(context, first, kBackgroundType, FloatRect(100, 100, 300, 300));
      DrawRect(context, first, kForegroundType, FloatRect(100, 100, 300, 300));
    }
    GetPaintController().CommitNewDisplayItems();
  }
