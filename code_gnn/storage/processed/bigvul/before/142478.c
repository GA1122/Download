void ShelfLayoutManager::SetChildBounds(aura::Window* child,
                                        const gfx::Rect& requested_bounds) {
  wm::WmSnapToPixelLayoutManager::SetChildBounds(child, requested_bounds);
  if (!updating_bounds_ &&
      ((shelf_widget_->GetNativeWindow() == child) ||
       (shelf_widget_->status_area_widget()->GetNativeWindow() == child))) {
    LayoutShelf();
  }
}
