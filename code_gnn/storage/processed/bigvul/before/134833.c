  void FinishCopyFromBackingStore(const base::Closure& quit_closure,
                                  bool frame_captured,
                                  const SkBitmap& bitmap) {
    ++callback_invoke_count_;
    if (frame_captured) {
      ++frames_captured_;
      EXPECT_FALSE(bitmap.empty());
    }
    if (!quit_closure.is_null())
      quit_closure.Run();
  }
