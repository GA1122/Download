void TaskQueueManager::PumpQueue(size_t queue_index) {
  DCHECK(main_thread_checker_.CalledOnValidThread());
  internal::TaskQueue* queue = Queue(queue_index);
  queue->PumpQueue();
}
