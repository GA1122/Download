  gfx::Rect GetSplitViewRightWindowBounds(aura::Window* window) {
    return split_view_controller()->GetSnappedWindowBoundsInScreen(
        window, SplitViewController::RIGHT);
  }
