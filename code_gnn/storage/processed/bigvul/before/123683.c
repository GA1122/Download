void GpuCommandBufferStub::PollWork() {
  TRACE_EVENT0("gpu", "GpuCommandBufferStub::PollWork");
  delayed_work_scheduled_ = false;
  FastSetActiveURL(active_url_, active_url_hash_);
  if (decoder_.get() && !MakeCurrent())
    return;
  if (scheduler_.get())
    scheduler_->PollUnscheduleFences();
  ScheduleDelayedWork(kHandleMoreWorkPeriodBusyMs);
}
