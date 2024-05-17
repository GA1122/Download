  void TestLessDrawing() {
    FakeDisplayItemClient first("first");
    GraphicsContext context(GetPaintController());

    InitRootChunk();
    DrawRect(context, first, kBackgroundType, FloatRect(100, 100, 300, 300));
    DrawRect(context, first, kForegroundType, FloatRect(100, 100, 300, 300));
    GetPaintController().CommitNewDisplayItems();

    InitRootChunk();
    DrawRect(context, first, kBackgroundType, FloatRect(100, 100, 300, 300));
    GetPaintController().CommitNewDisplayItems();
  }
