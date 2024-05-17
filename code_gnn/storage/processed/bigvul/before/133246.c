void TaskQueueManager::SetQueueName(size_t queue_index, const char* name) {
  DCHECK(main_thread_checker_.CalledOnValidThread());
  internal::TaskQueue* queue = Queue(queue_index);
  queue->set_name(name);
}
