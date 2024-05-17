bool TaskService::IsOnTaskRunner(RunnerId runner_id) {
  base::AutoLock lock(lock_);
  if (bound_instance_id_ == kInvalidInstanceId)
    return false;

  if (runner_id == kDefaultRunnerId)
    return default_task_runner_->BelongsToCurrentThread();

  size_t thread = runner_id - 1;
  if (threads_.size() <= thread || !threads_[thread])
    return false;

  return threads_[thread]->task_runner()->BelongsToCurrentThread();
}
