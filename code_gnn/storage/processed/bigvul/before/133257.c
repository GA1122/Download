bool TaskQueueManager::UpdateWorkQueues(
    bool should_trigger_wakeup,
    const base::PendingTask* previous_task) {
  DCHECK(main_thread_checker_.CalledOnValidThread());
  internal::LazyNow lazy_now(this);
  bool has_work = false;
  for (auto& queue : queues_) {
    has_work |=
        queue->UpdateWorkQueue(&lazy_now, should_trigger_wakeup, previous_task);
    if (!queue->work_queue().empty()) {
      DCHECK(queue->work_queue().front().delayed_run_time.is_null());
    }
  }
  return has_work;
}
