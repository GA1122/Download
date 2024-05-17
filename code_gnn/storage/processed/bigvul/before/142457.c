void ShelfLayoutManager::OnDisplayMetricsChanged(
    const display::Display& display,
    uint32_t changed_metrics) {
  TargetBounds target_bounds;
  CalculateTargetBoundsAndUpdateWorkArea(&target_bounds);
}
