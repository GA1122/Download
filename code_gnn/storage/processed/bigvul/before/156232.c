void RendererSchedulerImpl::OnQueueingTimeForWindowEstimated(
    base::TimeDelta queueing_time,
    bool is_disjoint_window) {
  main_thread_only().most_recent_expected_queueing_time = queueing_time;

  if (main_thread_only().has_navigated) {
    if (main_thread_only().max_queueing_time < queueing_time) {
      if (!main_thread_only().max_queueing_time_metric) {
        main_thread_only().max_queueing_time_metric =
            CreateMaxQueueingTimeMetric();
      }
      main_thread_only().max_queueing_time_metric->SetSample(
          queueing_time.InMilliseconds());
      main_thread_only().max_queueing_time = queueing_time;
    }
  }

  if (!is_disjoint_window)
    return;

  UMA_HISTOGRAM_TIMES("RendererScheduler.ExpectedTaskQueueingDuration",
                      queueing_time);
  UMA_HISTOGRAM_CUSTOM_COUNTS(
      "RendererScheduler.ExpectedTaskQueueingDuration2",
      queueing_time.InMicroseconds(), kMinExpectedQueueingTimeBucket,
      kMaxExpectedQueueingTimeBucket, kNumberExpectedQueueingTimeBuckets);
  TRACE_COUNTER1(TRACE_DISABLED_BY_DEFAULT("renderer.scheduler"),
                 "estimated_queueing_time_for_window",
                 queueing_time.InMillisecondsF());

  if (::resource_coordinator::IsResourceCoordinatorEnabled()) {
    RendererResourceCoordinator::Get().SetExpectedTaskQueueingDuration(
        queueing_time);
  }
}
