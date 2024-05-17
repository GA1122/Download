  void CheckWindowBounds(
      const gfx::Rect& bounds,
      const browser::WindowState state,
      std::unique_ptr<browser::GetWindowBoundsResult> result) {
    const browser::Bounds* actual_bounds = result->GetBounds();
#if !defined(OS_MACOSX)
    EXPECT_EQ(bounds.x(), actual_bounds->GetLeft());
    EXPECT_EQ(bounds.y(), actual_bounds->GetTop());
#endif   
    EXPECT_EQ(bounds.width(), actual_bounds->GetWidth());
    EXPECT_EQ(bounds.height(), actual_bounds->GetHeight());
    EXPECT_EQ(state, actual_bounds->GetWindowState());
  }
