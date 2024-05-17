uint64 SchedulerHelper::GetAndClearTaskWasRunOnQueueBitmap() {
  CheckOnValidThread();
  return task_queue_manager_->GetAndClearTaskWasRunOnQueueBitmap();
}
