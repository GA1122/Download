void TabLifecycleUnitSource::TabLifecycleUnit::OnLifecycleUnitStateChanged(
    LifecycleUnitState last_state,
    LifecycleUnitStateChangeReason reason) {
  DCHECK(IsValidStateChange(last_state, GetState(), reason))
      << "Cannot transition TabLifecycleUnit state from " << last_state
      << " to " << GetState() << " with reason " << reason;

  const bool was_discarded = IsDiscardedOrPendingDiscard(last_state);
  const bool is_discarded = IsDiscardedOrPendingDiscard(GetState());
  if (was_discarded != is_discarded) {
    for (auto& observer : *observers_)
      observer.OnDiscardedStateChange(GetWebContents(), is_discarded);
  }
}
