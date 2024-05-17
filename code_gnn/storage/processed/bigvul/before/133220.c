bool TaskQueueManager::IsQueueEmpty(size_t queue_index) const {
  return Queue(queue_index)->GetQueueState() == QueueState::EMPTY;
}
