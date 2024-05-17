double GetEventTimestamp(const Maybe<double>& timestamp) {
  return (GetEventTimeTicks(timestamp) - base::TimeTicks()).InSecondsF();
}
