bool TaskQueue::RunsTasksOnCurrentThread() const {
  base::AutoLock lock(lock_);
  return base::PlatformThread::CurrentId() == thread_id_;
}
