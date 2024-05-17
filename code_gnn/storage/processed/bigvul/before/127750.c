base::Time MockNetworkLayer::Now() {
  if (clock_)
    return clock_->Now();
  return base::Time::Now();
}
