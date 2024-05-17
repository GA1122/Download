scoped_refptr<MainThreadTaskQueue> RendererSchedulerImpl::NewTimerTaskQueue(
    MainThreadTaskQueue::QueueType queue_type) {
  DCHECK_EQ(MainThreadTaskQueue::QueueClassForQueueType(queue_type),
            MainThreadTaskQueue::QueueClass::kTimer);
  return NewTaskQueue(MainThreadTaskQueue::QueueCreationParams(queue_type)
                          .SetShouldReportWhenExecutionBlocked(true)
                          .SetCanBePaused(true)
                          .SetCanBeStopped(true)
                          .SetCanBeDeferred(true)
                          .SetCanBeThrottled(true));
}
