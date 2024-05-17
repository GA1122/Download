base::TimeTicks GetEventTimeTicks(const Maybe<double>& timestamp) {
  return timestamp.isJust()
             ? base::TimeDelta::FromSecondsD(timestamp.fromJust()) +
                   base::TimeTicks::UnixEpoch()
             : base::TimeTicks::Now();
}
