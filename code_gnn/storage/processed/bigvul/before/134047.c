void AppResult::UpdateFromLastLaunched(const base::Time& current_time,
                                       const base::Time& last_launched) {
  base::TimeDelta delta = current_time - last_launched;
  if (current_time < last_launched) {
    set_relevance(1.0);
    return;
  }

  const int kSecondsInWeek = 60 * 60 * 24 * 7;

  set_relevance(1 / (1 + delta.InSecondsF() / kSecondsInWeek));
}
