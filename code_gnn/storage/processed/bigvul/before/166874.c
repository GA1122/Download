void Performance::ActivateObserver(PerformanceObserver& observer) {
  if (active_observers_.IsEmpty())
    deliver_observations_timer_.StartOneShot(TimeDelta(), FROM_HERE);

  active_observers_.insert(&observer);
}
