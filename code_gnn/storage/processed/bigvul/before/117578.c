double LimitPercent(double percent) {
  return min(max(percent, 0.0), 100.0);
}
