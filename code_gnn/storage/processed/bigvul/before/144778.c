    EnsureFreezeTimeoutTimerInitialized() {
  if (!freeze_timeout_timer_) {
    freeze_timeout_timer_ =
        std::make_unique<base::OneShotTimer>(GetTickClock());
  }
}
