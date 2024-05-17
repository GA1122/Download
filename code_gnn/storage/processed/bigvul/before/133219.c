TaskQueueManager::QueueState TaskQueueManager::GetQueueState(size_t queue_index)
    const {
  return Queue(queue_index)->GetQueueState();
}
