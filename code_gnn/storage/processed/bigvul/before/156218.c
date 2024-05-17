bool RendererSchedulerImpl::MainThreadSeemsUnresponsive(
    base::TimeDelta main_thread_responsiveness_threshold) {
  base::TimeTicks now = tick_clock()->NowTicks();
  base::TimeDelta estimated_queueing_time;

  bool can_read = false;

  base::subtle::Atomic32 version;
  seqlock_queueing_time_estimator_.seqlock.TryRead(&can_read, &version);

  if (!can_read)
    return GetCompositorThreadOnly().main_thread_seems_unresponsive;

  QueueingTimeEstimator::State queueing_time_estimator_state =
      seqlock_queueing_time_estimator_.data.GetState();

  if (seqlock_queueing_time_estimator_.seqlock.ReadRetry(version))
    return GetCompositorThreadOnly().main_thread_seems_unresponsive;

  QueueingTimeEstimator queueing_time_estimator(queueing_time_estimator_state);

  estimated_queueing_time =
      queueing_time_estimator.EstimateQueueingTimeIncludingCurrentTask(now);

  bool main_thread_seems_unresponsive =
      estimated_queueing_time > main_thread_responsiveness_threshold;
  GetCompositorThreadOnly().main_thread_seems_unresponsive =
      main_thread_seems_unresponsive;

  return main_thread_seems_unresponsive;
}
