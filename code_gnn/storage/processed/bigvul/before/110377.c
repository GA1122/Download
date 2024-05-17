PP_TimeTicks GetTickTime() {
  return TimeTicksToPPTimeTicks(base::TimeTicks::Now());
}
