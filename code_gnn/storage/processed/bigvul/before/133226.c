base::TimeTicks TaskQueueManager::NextPendingDelayedTaskRunTime() {
  DCHECK(main_thread_checker_.CalledOnValidThread());
  bool found_pending_task = false;
  base::TimeTicks next_pending_delayed_task(
      base::TimeTicks::FromInternalValue(kMaxTimeTicks));
  for (auto& queue : queues_) {
    base::TimeTicks queues_next_pending_delayed_task;
    if (queue->NextPendingDelayedTaskRunTime(
            &queues_next_pending_delayed_task)) {
      found_pending_task = true;
      next_pending_delayed_task =
          std::min(next_pending_delayed_task, queues_next_pending_delayed_task);
    }
  }

  if (!found_pending_task)
    return base::TimeTicks();

  DCHECK_NE(next_pending_delayed_task,
            base::TimeTicks::FromInternalValue(kMaxTimeTicks));
  return next_pending_delayed_task;
}
