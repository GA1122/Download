void QuotaThreadTask::CallRunOnTargetThread() {
  DCHECK(target_task_runner_->RunsTasksOnCurrentThread());
  if (RunOnTargetThreadAsync())
    original_task_runner()->PostTask(
        FROM_HERE,
        base::Bind(&QuotaThreadTask::CallCompleted, this));
}
