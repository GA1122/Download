gfx::Size ShelfLayoutManager::GetPreferredSize() {
  TargetBounds target_bounds;
  CalculateTargetBoundsAndUpdateWorkArea(&target_bounds);
  return target_bounds.shelf_bounds.size();
}
