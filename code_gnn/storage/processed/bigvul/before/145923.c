  void IsPanelAboveLauncherIcon(aura::Window* panel) {
    shelf_view_test()->RunMessageLoopUntilAnimationsDone();

    Shelf* shelf = GetShelfForWindow(panel);
    gfx::Rect icon_bounds = shelf->GetScreenBoundsOfItemIconForWindow(panel);
    ASSERT_FALSE(icon_bounds.width() == 0 && icon_bounds.height() == 0);

    gfx::Rect window_bounds = panel->GetBoundsInScreen();
    ASSERT_LT(icon_bounds.width(), window_bounds.width());
    ASSERT_LT(icon_bounds.height(), window_bounds.height());
    gfx::Rect shelf_bounds = shelf->shelf_widget()->GetWindowBoundsInScreen();
    const ShelfAlignment alignment = shelf->alignment();

    if (IsHorizontal(alignment)) {
      EXPECT_LE(window_bounds.x(), icon_bounds.x());
      EXPECT_GE(window_bounds.right(), icon_bounds.right());
    } else {
      EXPECT_LE(window_bounds.y(), icon_bounds.y());
      EXPECT_GE(window_bounds.bottom(), icon_bounds.bottom());
    }

    if (alignment == SHELF_ALIGNMENT_LEFT)
      EXPECT_EQ(shelf_bounds.right(), window_bounds.x());
    else if (alignment == SHELF_ALIGNMENT_RIGHT)
      EXPECT_EQ(shelf_bounds.x(), window_bounds.right());
    else
      EXPECT_EQ(shelf_bounds.y(), window_bounds.bottom());
  }
