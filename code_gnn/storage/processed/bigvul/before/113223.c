gfx::Rect ShelfLayoutManager::GetMaximizedWindowBounds(
    aura::Window* window) {
  gfx::Rect bounds(gfx::Screen::GetDisplayNearestWindow(window).bounds());
  if (auto_hide_behavior_ == SHELF_AUTO_HIDE_BEHAVIOR_DEFAULT ||
      auto_hide_behavior_ == SHELF_AUTO_HIDE_BEHAVIOR_ALWAYS) {
    AdjustBoundsBasedOnAlignment(kAutoHideSize, &bounds);
    return bounds;
  }
  return GetUnmaximizedWorkAreaBounds(window);
}
