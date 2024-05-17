void MockNetworkLayer::SetClock(base::Clock* clock) {
  DCHECK(!clock_);
  clock_ = clock;
}
