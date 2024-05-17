void Resource::FinishAsError(const ResourceError& error,
                             base::SingleThreadTaskRunner* task_runner) {
  error_ = error;
  is_revalidating_ = false;

  if (IsMainThread())
    GetMemoryCache()->Remove(this);

  if (!ErrorOccurred())
    SetStatus(ResourceStatus::kLoadError);
  DCHECK(ErrorOccurred());
  ClearData();
  loader_ = nullptr;
  CheckResourceIntegrity();
  TriggerNotificationForFinishObservers(task_runner);
  NotifyFinished();
}
