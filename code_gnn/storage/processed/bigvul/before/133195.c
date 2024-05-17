uint64 SchedulerHelper::GetQuiescenceMonitoredTaskQueueMask() const {
  CheckOnValidThread();
  return quiescence_monitored_task_queue_mask_;
}
