void TaskQueueManager::SetPumpPolicy(size_t queue_index,
                                     PumpPolicy pump_policy) {
  DCHECK(main_thread_checker_.CalledOnValidThread());
  internal::TaskQueue* queue = Queue(queue_index);
  queue->SetPumpPolicy(pump_policy);
}
