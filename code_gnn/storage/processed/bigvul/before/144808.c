void TabLifecycleUnitSource::TabLifecycleUnit::UpdateLifecycleState(
    mojom::LifecycleState state) {
  switch (state) {
    case mojom::LifecycleState::kFrozen: {
      if (GetState() == LifecycleUnitState::PENDING_DISCARD) {
        freeze_timeout_timer_->Stop();
        FinishDiscard(discard_reason_);
      } else {
        SetState(LifecycleUnitState::FROZEN,
                 StateChangeReason::RENDERER_INITIATED);
      }
      break;
    }

    case mojom::LifecycleState::kRunning: {
      SetState(LifecycleUnitState::ACTIVE,
               StateChangeReason::RENDERER_INITIATED);
      break;
    }

    default: {
      NOTREACHED();
      break;
    }
  }
}
