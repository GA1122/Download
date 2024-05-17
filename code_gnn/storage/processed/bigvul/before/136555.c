  void TestInvalidationInSubsequence() {
    FakeDisplayItemClient container("container");
    FakeDisplayItemClient content("content");
    GraphicsContext context(GetPaintController());

    InitRootChunk();
    {
      SubsequenceRecorder r(context, container);
      DrawRect(context, content, kBackgroundType,
               FloatRect(100, 100, 300, 300));
    }
    GetPaintController().CommitNewDisplayItems();

    content.SetDisplayItemsUncached();
    InitRootChunk();
    {
      EXPECT_FALSE(SubsequenceRecorder::UseCachedSubsequenceIfPossible(
          context, container));
      SubsequenceRecorder r(context, container);
      DrawRect(context, content, kBackgroundType,
               FloatRect(100, 100, 300, 300));
    }
    GetPaintController().CommitNewDisplayItems();
  }
