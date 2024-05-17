void ChromeInvalidationClient::WriteState(const std::string& state) {
  DCHECK(CalledOnValidThread());
  DVLOG(1) << "WriteState";
  invalidation_state_tracker_.Call(
       FROM_HERE, &InvalidationStateTracker::SetInvalidationState, state);
 }
