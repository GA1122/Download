void TaskService::PostBoundTask(RunnerId runner_id, base::OnceClosure task) {
  InstanceId instance_id;
  {
    base::AutoLock lock(lock_);
    if (bound_instance_id_ == kInvalidInstanceId)
      return;
    instance_id = bound_instance_id_;
  }
  GetTaskRunner(runner_id)->PostTask(
      FROM_HERE, base::BindOnce(&TaskService::RunTask, base::Unretained(this),
                                instance_id, runner_id, std::move(task)));
}
