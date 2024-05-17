void ExpectCanDiscardTrue(const LifecycleUnit* lifecycle_unit,
                          DiscardReason discard_reason) {
  DecisionDetails decision_details;
  EXPECT_TRUE(lifecycle_unit->CanDiscard(discard_reason, &decision_details));
  EXPECT_TRUE(decision_details.IsPositive());
  EXPECT_EQ(1u, decision_details.reasons().size());
  EXPECT_EQ(DecisionSuccessReason::HEURISTIC_OBSERVED_TO_BE_SAFE,
            decision_details.SuccessReason());
}
