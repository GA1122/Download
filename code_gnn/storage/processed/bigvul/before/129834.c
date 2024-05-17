void TraceEvent::UpdateDuration(const TraceTicks& now,
                                const ThreadTicks& thread_now) {
  DCHECK_EQ(duration_.ToInternalValue(), -1);
  duration_ = now - timestamp_;

  if (thread_timestamp_ != ThreadTicks())
    thread_duration_ = thread_now - thread_timestamp_;
}
