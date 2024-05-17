void JankTimeBomb::Alarm(base::PlatformThreadId thread_id) {
  DCHECK(WatchDogThread::CurrentlyOnWatchDogThread());
  sampling_profiler_.reset(new base::StackSamplingProfiler(
      thread_id,
      GetJankTimeBombSamplingParams(),
      metrics::CallStackProfileMetricsProvider::GetProfilerCallback(
          metrics::CallStackProfileMetricsProvider::Params(
              metrics::CallStackProfileMetricsProvider::JANKY_TASK,
              true))));
  sampling_profiler_->Start();
}
