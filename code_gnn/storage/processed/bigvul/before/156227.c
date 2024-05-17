void RendererSchedulerImpl::OnBeginNestedRunLoop() {
  seqlock_queueing_time_estimator_.seqlock.WriteBegin();
  seqlock_queueing_time_estimator_.data.OnBeginNestedRunLoop();
  seqlock_queueing_time_estimator_.seqlock.WriteEnd();

  main_thread_only().nested_runloop = true;
  ApplyVirtualTimePolicy();
}
