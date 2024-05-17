void BaseAudioContext::ScheduleMainThreadCleanup() {
  if (has_posted_cleanup_task_)
    return;
  PostCrossThreadTask(
      *task_runner_, FROM_HERE,
      CrossThreadBind(&BaseAudioContext::PerformCleanupOnMainThread,
                      WrapCrossThreadPersistent(this)));
  has_posted_cleanup_task_ = true;
}
