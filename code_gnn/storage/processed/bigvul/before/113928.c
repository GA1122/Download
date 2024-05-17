void InvalidationNotifier::SetStateDeprecated(const std::string& state) {
  DCHECK(CalledOnValidThread());
  DCHECK_LT(state_, STARTED);
  if (invalidation_state_.empty()) {
    invalidation_state_ = state;
    invalidation_state_tracker_.Call(
        FROM_HERE, &InvalidationStateTracker::SetInvalidationState, state);
    UMA_HISTOGRAM_BOOLEAN("InvalidationNotifier.UsefulSetState", true);
  } else {
    UMA_HISTOGRAM_BOOLEAN("InvalidationNotifier.UsefulSetState", false);
  }
}
