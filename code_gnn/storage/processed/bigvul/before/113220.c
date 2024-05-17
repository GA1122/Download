gfx::Rect ShelfLayoutManager::GetIdealBounds() {
  gfx::Rect bounds(
      gfx::Screen::GetDisplayNearestWindow(status_->GetNativeView()).bounds());
  int width = 0, height = 0;
  GetShelfSize(&width, &height);
  switch (alignment_) {
    case SHELF_ALIGNMENT_BOTTOM:
      return gfx::Rect(bounds.x(), bounds.bottom() - height,
                       bounds.width(), height);
    case SHELF_ALIGNMENT_LEFT:
      return gfx::Rect(bounds.x(), bounds.y(), width, bounds.height());
    case SHELF_ALIGNMENT_RIGHT:
      return gfx::Rect(bounds.right() - width, bounds.y(), width,
                       bounds.height());
  }
  NOTREACHED();
  return gfx::Rect();
}
