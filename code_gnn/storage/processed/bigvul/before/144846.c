 void ExpectCanDiscardFalseTrivialAllReasons(
    const LifecycleUnit* lifecycle_unit) {
  ExpectCanDiscardFalseTrivial(lifecycle_unit, DiscardReason::kExternal);
  ExpectCanDiscardFalseTrivial(lifecycle_unit, DiscardReason::kProactive);
  ExpectCanDiscardFalseTrivial(lifecycle_unit, DiscardReason::kUrgent);
}
