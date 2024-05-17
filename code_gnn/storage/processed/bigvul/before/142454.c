void ShelfLayoutManager::LayoutShelfAndUpdateBounds() {
  TargetBounds target_bounds;
  CalculateTargetBoundsAndUpdateWorkArea(&target_bounds);
  UpdateBoundsAndOpacity(target_bounds, false, nullptr);

  for (auto& observer : observers_)
    observer.WillChangeVisibilityState(visibility_state());
}
