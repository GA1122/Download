TaskQueue::QueuePriority RendererSchedulerImpl::TaskQueuePolicy::GetPriority(
    MainThreadTaskQueue* task_queue) const {
  return task_queue->UsedForControlTasks() ? TaskQueue::kHighPriority
                                           : priority;
}
