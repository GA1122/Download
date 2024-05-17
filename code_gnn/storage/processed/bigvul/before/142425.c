void ShelfLayoutManager::CalculateTargetBoundsAndUpdateWorkArea(
    TargetBounds* target_bounds) {
  user_work_area_bounds_ = CalculateTargetBounds(state_, target_bounds);
}
