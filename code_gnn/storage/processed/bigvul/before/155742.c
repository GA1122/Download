void Compositor::SetDisplayVSyncParameters(base::TimeTicks timebase,
                                           base::TimeDelta interval) {
  static bool is_frame_rate_limit_disabled =
      base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableFrameRateLimit);
  if (is_frame_rate_limit_disabled)
    return;

  if (forced_refresh_rate_) {
    timebase = base::TimeTicks();
    interval = base::TimeDelta::FromSeconds(1) / forced_refresh_rate_;
  }
  if (interval.is_zero()) {
    interval = viz::BeginFrameArgs::DefaultInterval();
  }
  DCHECK_GT(interval.InMillisecondsF(), 0);

  if (vsync_timebase_ == timebase && vsync_interval_ == interval)
    return;

  vsync_timebase_ = timebase;
  vsync_interval_ = interval;
  refresh_rate_ =
      base::Time::kMillisecondsPerSecond / interval.InMillisecondsF();
  if (context_factory_private_) {
    context_factory_private_->SetDisplayVSyncParameters(this, timebase,
                                                        interval);
  }
  vsync_manager_->UpdateVSyncParameters(timebase, interval);
}
