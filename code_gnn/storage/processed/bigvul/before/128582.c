bool Instance::HandleDocumentLoad(const pp::URLLoader& loader) {
  delayed_progress_timer_id_ = ScheduleTimer(kProgressBarId,
                                             kProgressDelayTimeoutMs);
  return engine_->HandleDocumentLoad(loader);
}
