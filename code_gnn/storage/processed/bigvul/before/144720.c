void TaskService::PostStaticTask(RunnerId runner_id, base::OnceClosure task) {
  {
    base::AutoLock lock(lock_);
    if (bound_instance_id_ == kInvalidInstanceId)
      return;
  }
  scoped_refptr<base::SingleThreadTaskRunner> runner;
  GetTaskRunner(runner_id)->PostTask(FROM_HERE, std::move(task));
}
