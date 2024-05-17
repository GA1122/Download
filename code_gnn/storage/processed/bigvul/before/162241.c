void GamepadProvider::ScheduleDoPoll() {
  DCHECK(polling_thread_->task_runner()->BelongsToCurrentThread());
  if (have_scheduled_do_poll_)
    return;

  {
    base::AutoLock lock(is_paused_lock_);
    if (is_paused_)
      return;
  }

  base::ThreadTaskRunnerHandle::Get()->PostDelayedTask(
      FROM_HERE, base::Bind(&GamepadProvider::DoPoll, Unretained(this)),
      base::TimeDelta::FromMilliseconds(kDesiredSamplingIntervalMs));
  have_scheduled_do_poll_ = true;
}
