  void TransitionFromPendingDiscardToDiscardedIfNeeded(
      DiscardReason reason,
      LifecycleUnit* lifecycle_unit) {
    if (reason == DiscardReason::kProactive) {
      EXPECT_EQ(LifecycleUnitState::PENDING_DISCARD,
                lifecycle_unit->GetState());
      task_runner_->FastForwardBy(kProactiveDiscardFreezeTimeout);
    }
    EXPECT_EQ(LifecycleUnitState::DISCARDED, lifecycle_unit->GetState());
  }
