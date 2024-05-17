SnapCoordinator* Document::GetSnapCoordinator() {
  if (RuntimeEnabledFeatures::CSSScrollSnapPointsEnabled() &&
      !snap_coordinator_)
    snap_coordinator_ = SnapCoordinator::Create();

  return snap_coordinator_.Get();
}
