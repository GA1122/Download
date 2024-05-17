void RendererSchedulerImpl::SetRendererBackgrounded(bool backgrounded) {
  helper_.CheckOnValidThread();
  if (helper_.IsShutdown() ||
      main_thread_only().renderer_backgrounded == backgrounded)
    return;
  if (backgrounded) {
    TRACE_EVENT0(TRACE_DISABLED_BY_DEFAULT("renderer.scheduler"),
                 "RendererSchedulerImpl::OnRendererBackgrounded");
    RendererMetricsHelper::RecordBackgroundedTransition(
        BackgroundedRendererTransition::kBackgrounded);
  } else {
    TRACE_EVENT0(TRACE_DISABLED_BY_DEFAULT("renderer.scheduler"),
                 "RendererSchedulerImpl::OnRendererForegrounded");
    RendererMetricsHelper::RecordBackgroundedTransition(
        BackgroundedRendererTransition::kForegrounded);
  }

  main_thread_only().renderer_backgrounded = backgrounded;
  internal::ProcessState::Get()->is_process_backgrounded = backgrounded;

  main_thread_only().background_status_changed_at = tick_clock()->NowTicks();
  seqlock_queueing_time_estimator_.seqlock.WriteBegin();
  seqlock_queueing_time_estimator_.data.OnRendererStateChanged(
      backgrounded, main_thread_only().background_status_changed_at);
  seqlock_queueing_time_estimator_.seqlock.WriteEnd();

  UpdatePolicy();

  base::TimeTicks now = tick_clock()->NowTicks();
  if (backgrounded) {
    main_thread_only().metrics_helper.OnRendererBackgrounded(now);
  } else {
    main_thread_only().metrics_helper.OnRendererForegrounded(now);
  }
}
