 void LayerTreeHost::StartRateLimiter() {
   if (animating_)
     return;

  if (!rate_limit_timer_.IsRunning()) {
    rate_limit_timer_.Start(FROM_HERE,
                            base::TimeDelta(),
                            this,
                            &LayerTreeHost::RateLimit);
  }
}
