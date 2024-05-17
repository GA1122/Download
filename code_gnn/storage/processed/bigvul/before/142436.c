gfx::Rect ShelfLayoutManager::GetIdealBounds() const {
  const int shelf_size = ShelfConstants::shelf_size();
  aura::Window* shelf_window = shelf_widget_->GetNativeWindow();
  gfx::Rect rect(screen_util::GetDisplayBoundsInParent(shelf_window));
  return SelectValueForShelfAlignment(
      gfx::Rect(rect.x(), rect.bottom() - shelf_size, rect.width(), shelf_size),
      gfx::Rect(rect.x(), rect.y(), shelf_size, rect.height()),
      gfx::Rect(rect.right() - shelf_size, rect.y(), shelf_size,
                rect.height()));
}
