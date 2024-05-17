NavigationRateLimiter::NavigationRateLimiter(Frame& frame)
    : frame_(frame),
      time_first_count_(base::TimeTicks::Now()),
      enabled(frame_->GetSettings()->GetShouldProtectAgainstIpcFlooding()) {}
