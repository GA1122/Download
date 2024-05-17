void ExpectCanDiscardFalseAllReasons(const LifecycleUnit* lifecycle_unit,
                                     DecisionFailureReason failure_reason) {
  ExpectCanDiscardFalse(lifecycle_unit, failure_reason,
                        DiscardReason::kExternal);
  ExpectCanDiscardFalse(lifecycle_unit, failure_reason,
                        DiscardReason::kProactive);
  ExpectCanDiscardFalse(lifecycle_unit, failure_reason, DiscardReason::kUrgent);
}
