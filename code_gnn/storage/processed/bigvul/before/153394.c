int GetDraggedX(const gfx::Rect& dragged_bounds) {
  return dragged_bounds.x() + TabStyle::GetTabInternalPadding().left();
}
