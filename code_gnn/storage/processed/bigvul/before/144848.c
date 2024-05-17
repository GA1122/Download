void ExpectCanDiscardTrueAllReasons(const LifecycleUnit* lifecycle_unit) {
  ExpectCanDiscardTrue(lifecycle_unit, DiscardReason::kExternal);
  ExpectCanDiscardTrue(lifecycle_unit, DiscardReason::kProactive);
  ExpectCanDiscardTrue(lifecycle_unit, DiscardReason::kUrgent);
}
