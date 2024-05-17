 scoped_refptr<base::SingleThreadTaskRunner> SchedulerHelper::TaskRunnerForQueue(
    size_t queue_index) const {
  CheckOnValidThread();
  return task_queue_manager_->TaskRunnerForQueue(queue_index);
}
