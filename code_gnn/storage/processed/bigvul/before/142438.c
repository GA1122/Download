gfx::Rect ShelfLayoutManager::GetVisibleShelfBounds() const {
  gfx::Rect shelf_region = shelf_widget_->GetWindowBoundsInScreen();
  DCHECK(!display_.bounds().IsEmpty());
  shelf_region.Intersect(display_.bounds());
  return screen_util::SnapBoundsToDisplayEdge(shelf_region,
                                              shelf_widget_->GetNativeWindow());
}
