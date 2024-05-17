TaskQueueManager::~TaskQueueManager() {
  TRACE_EVENT_OBJECT_DELETED_WITH_ID(disabled_by_default_tracing_category_,
                                     "TaskQueueManager", this);
  for (auto& queue : queues_)
    queue->WillDeleteTaskQueueManager();
  selector_->SetTaskQueueSelectorObserver(nullptr);
}
