void TaskQueueManager::OnTaskQueueEnabled() {
  DCHECK(main_thread_checker_.CalledOnValidThread());
  MaybePostDoWorkOnMainRunner();
}
