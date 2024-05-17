void Resource::Finish(double load_finish_time,
                      base::SingleThreadTaskRunner* task_runner) {
  DCHECK(!is_revalidating_);
  load_finish_time_ = load_finish_time;
  if (!ErrorOccurred())
    status_ = ResourceStatus::kCached;
  loader_ = nullptr;
  CheckResourceIntegrity();
  TriggerNotificationForFinishObservers(task_runner);
  NotifyFinished();
}
