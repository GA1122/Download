  gfx::SlideAnimation* GetBackgroundViewAnimationForWindow(
      int grid_index,
      aura::Window* window) {
    return GetWindowItemForWindow(grid_index, window)
        ->GetBackgroundViewAnimation();
  }
