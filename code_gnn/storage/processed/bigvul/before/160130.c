base::TimeDelta BackendIO::ElapsedTime() const {
  return base::TimeTicks::Now() - start_time_;
}
