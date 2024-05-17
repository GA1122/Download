bool TaskService::BindInstance() {
  base::AutoLock lock(lock_);
  if (bound_instance_id_ != kInvalidInstanceId)
    return false;
  bound_instance_id_ = next_instance_id_++;

  DCHECK(!default_task_runner_);
  default_task_runner_ = base::ThreadTaskRunnerHandle::Get();
  return true;
}
