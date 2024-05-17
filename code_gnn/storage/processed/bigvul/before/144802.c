void TabLifecycleUnitSource::TabLifecycleUnit::SetFocused(bool focused) {
  const bool was_focused = last_focused_time_ == base::TimeTicks::Max();
  if (focused == was_focused)
    return;
  last_focused_time_ = focused ? base::TimeTicks::Max() : NowTicks();

  if (!focused)
    return;

  switch (GetState()) {
    case LifecycleUnitState::DISCARDED: {
      SetState(LifecycleUnitState::ACTIVE,
               StateChangeReason::BROWSER_INITIATED);
      bool loaded = Load();
      DCHECK(loaded);
      break;
    }

    case LifecycleUnitState::PENDING_DISCARD: {
      freeze_timeout_timer_->Stop();
      SetState(LifecycleUnitState::PENDING_FREEZE,
               StateChangeReason::BROWSER_INITIATED);
      break;
    }

    default:
      break;
  }
}
