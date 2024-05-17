scoped_refptr<MainThreadTaskQueue> RendererSchedulerImpl::NewLoadingTaskQueue(
    MainThreadTaskQueue::QueueType queue_type) {
  DCHECK_EQ(MainThreadTaskQueue::QueueClassForQueueType(queue_type),
            MainThreadTaskQueue::QueueClass::kLoading);
  return NewTaskQueue(
      MainThreadTaskQueue::QueueCreationParams(queue_type)
          .SetCanBePaused(true)
          .SetCanBeStopped(StopLoadingInBackgroundEnabled())
          .SetCanBeDeferred(true)
          .SetUsedForControlTasks(
              queue_type ==
              MainThreadTaskQueue::QueueType::kFrameLoadingControl));
}
