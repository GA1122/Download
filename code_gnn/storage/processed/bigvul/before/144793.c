bool TabLifecycleUnitSource::TabLifecycleUnit::IsDiscarded() const {
  return IsDiscardedOrPendingDiscard(GetState());
}
