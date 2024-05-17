  base::TimeDelta InFuture(int time_in_millis) {
    return base::TimeDelta::FromMilliseconds(simulated_now_ + time_in_millis);
  }
