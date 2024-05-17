void ExpectCanDiscardFalse(const LifecycleUnit* lifecycle_unit,
                           DecisionFailureReason failure_reason,
                           DiscardReason discard_reason) {
  DecisionDetails decision_details;
  EXPECT_FALSE(lifecycle_unit->CanDiscard(discard_reason, &decision_details));
  EXPECT_FALSE(decision_details.IsPositive());
  EXPECT_EQ(1u, decision_details.reasons().size());
  EXPECT_EQ(failure_reason, decision_details.FailureReason());
}
