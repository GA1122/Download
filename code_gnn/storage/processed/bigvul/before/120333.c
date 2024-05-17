  base::TimeDelta Now() {
    base::TimeDelta t = base::TimeDelta::FromMilliseconds(simulated_now_);
    simulated_now_++;
    return t;
  }
