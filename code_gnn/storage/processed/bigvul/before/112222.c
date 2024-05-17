void QuotaThreadTask::Run() {
  target_task_runner_->PostTask(
      FROM_HERE,
      base::Bind(&QuotaThreadTask::CallRunOnTargetThread, this));
}
