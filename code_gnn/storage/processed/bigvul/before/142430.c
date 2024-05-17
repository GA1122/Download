gfx::Rect ShelfLayoutManager::ComputeStableWorkArea() const {
  TargetBounds target_bounds;
  State state = state_;
  state.visibility_state = SHELF_VISIBLE;
  return CalculateTargetBounds(state, &target_bounds);
}
