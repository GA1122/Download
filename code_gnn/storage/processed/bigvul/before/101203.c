void IncrementErrorCounters(StatusController* status) {
  status->increment_num_consecutive_errors();
}
