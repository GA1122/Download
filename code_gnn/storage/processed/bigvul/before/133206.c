void SchedulerHelper::SetWorkBatchSizeForTesting(size_t work_batch_size) {
  CheckOnValidThread();
  task_queue_manager_->SetWorkBatchSize(work_batch_size);
}
