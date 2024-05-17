  void PanelsNotOverlapping(aura::Window* panel1, aura::Window* panel2) {
    shelf_view_test()->RunMessageLoopUntilAnimationsDone();
    gfx::Rect window1_bounds = panel1->GetBoundsInRootWindow();
    gfx::Rect window2_bounds = panel2->GetBoundsInRootWindow();

    EXPECT_FALSE(window1_bounds.Intersects(window2_bounds));
  }
