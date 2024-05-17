void TabLifecycleUnitSource::TabLifecycleUnit::DidStartLoading() {
  if (IsDiscardedOrPendingDiscard(GetState()))
    SetState(LifecycleUnitState::ACTIVE, StateChangeReason::BROWSER_INITIATED);
}
