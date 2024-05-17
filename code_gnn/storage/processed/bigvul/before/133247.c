void TaskQueueManager::SetWakeupPolicy(size_t queue_index,
                                       WakeupPolicy wakeup_policy) {
  DCHECK(main_thread_checker_.CalledOnValidThread());
  internal::TaskQueue* queue = Queue(queue_index);
  queue->set_wakeup_policy(wakeup_policy);
}
