void SchedulerHelper::Shutdown() {
  CheckOnValidThread();
  task_queue_manager_.reset();
}
