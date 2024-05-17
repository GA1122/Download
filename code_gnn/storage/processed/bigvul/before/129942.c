ScopedDisablePriorityBoost::~ScopedDisablePriorityBoost() {
  if (got_previous_boost_state_)
     ::SetThreadPriorityBoost(thread_handle_, boost_state_was_disabled_);
 }
