  void TestChangeDrawing() {
    FakeDisplayItemClient first("first");
    GraphicsContext context(GetPaintController());

    InitRootChunk();
    first.SetVisualRect(LayoutRect(100, 100, 300, 300));
    DrawRect(context, first, kBackgroundType, FloatRect(100, 100, 300, 300));
    DrawRect(context, first, kForegroundType, FloatRect(100, 100, 300, 300));
    GetPaintController().CommitNewDisplayItems();

    InitRootChunk();
    first.SetVisualRect(LayoutRect(200, 200, 300, 300));
    DrawRect(context, first, kBackgroundType, FloatRect(200, 200, 300, 300));
    DrawRect(context, first, kForegroundType, FloatRect(100, 100, 300, 300));
    GetPaintController().CommitNewDisplayItems();
  }
