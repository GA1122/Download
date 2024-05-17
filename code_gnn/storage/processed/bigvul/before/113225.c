gfx::Rect ShelfLayoutManager::GetUnmaximizedWorkAreaBounds(
    aura::Window* window) {
  gfx::Rect bounds(gfx::Screen::GetDisplayNearestWindow(window).bounds());
  int size;
  if (auto_hide_behavior_ == SHELF_AUTO_HIDE_BEHAVIOR_ALWAYS) {
    size = kAutoHideSize;
  } else {
    int width, height;
    GetShelfSize(&width, &height);
    size = std::max(width, height);
  }
  AdjustBoundsBasedOnAlignment(size, &bounds);
  return bounds;
}
