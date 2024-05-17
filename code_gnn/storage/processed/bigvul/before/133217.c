uint64 TaskQueueManager::GetAndClearTaskWasRunOnQueueBitmap() {
  uint64 bitmap = task_was_run_bitmap_;
  task_was_run_bitmap_ = 0;
  return bitmap;
 }
