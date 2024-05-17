bool IsDiscardedOrPendingDiscard(LifecycleUnitState state) {
  return state == LifecycleUnitState::DISCARDED ||
         state == LifecycleUnitState::PENDING_DISCARD;
}
