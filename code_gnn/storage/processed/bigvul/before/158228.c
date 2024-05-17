void AppCacheGroup::SetUpdateAppCacheStatus(UpdateAppCacheStatus status) {
  if (status == update_status_)
    return;

  update_status_ = status;

  if (status != IDLE) {
    DCHECK(update_job_);
  } else {
    update_job_ = nullptr;

    scoped_refptr<AppCacheGroup> protect(is_in_dtor_ ? nullptr : this);
    for (auto& observer : observers_)
      observer.OnUpdateComplete(this);
    if (!queued_updates_.empty())
      ScheduleUpdateRestart(kUpdateRestartDelayMs);
  }
}
