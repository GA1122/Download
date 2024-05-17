  gfx::Rect GetSplitViewLeftWindowBounds(aura::Window* window) {
    return split_view_controller()->GetSnappedWindowBoundsInScreen(
        window, SplitViewController::LEFT);
  }
