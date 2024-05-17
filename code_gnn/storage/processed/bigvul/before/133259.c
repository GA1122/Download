void TaskQueue::WillDeleteTaskQueueManager() {
  base::AutoLock lock(lock_);
  task_queue_manager_ = nullptr;
  delayed_task_queue_ = base::DelayedTaskQueue();
  incoming_queue_ = base::TaskQueue();
  work_queue_ = base::TaskQueue();
}
