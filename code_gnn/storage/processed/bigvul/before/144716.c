scoped_refptr<base::SingleThreadTaskRunner> TaskService::GetTaskRunner(
    RunnerId runner_id) {
  base::AutoLock lock(lock_);
  if (runner_id == kDefaultRunnerId)
    return default_task_runner_;

  if (threads_.size() < runner_id)
    threads_.resize(runner_id);

  size_t thread = runner_id - 1;
  if (!threads_[thread]) {
    threads_[thread] = base::MakeUnique<base::Thread>(
        base::StringPrintf("MidiService_TaskService_Thread(%zu)", runner_id));
#if defined(OS_WIN)
    threads_[thread]->init_com_with_mta(true);
#endif
    threads_[thread]->Start();
  }
  return threads_[thread]->task_runner();
}
