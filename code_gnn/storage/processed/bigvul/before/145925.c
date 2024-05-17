  void PanelInScreen(aura::Window* panel) {
    gfx::Rect panel_bounds = panel->GetBoundsInRootWindow();
    gfx::Point root_point = gfx::Point(panel_bounds.x(), panel_bounds.y());
    display::Display display =
        display::Screen::GetScreen()->GetDisplayNearestPoint(root_point);

    gfx::Rect panel_bounds_in_screen = panel->GetBoundsInScreen();
    gfx::Point screen_bottom_right = gfx::Point(
        panel_bounds_in_screen.right(), panel_bounds_in_screen.bottom());
    gfx::Rect display_bounds = display.bounds();
    EXPECT_TRUE(screen_bottom_right.x() < display_bounds.width() &&
                screen_bottom_right.y() < display_bounds.height());
  }
