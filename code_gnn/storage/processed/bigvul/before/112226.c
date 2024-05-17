void QuotaTaskObserver::UnregisterTask(QuotaTask* task) {
  DCHECK(running_quota_tasks_.find(task) != running_quota_tasks_.end());
  running_quota_tasks_.erase(task);
}
