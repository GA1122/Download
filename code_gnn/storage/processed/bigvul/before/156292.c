RendererSchedulerImpl::~RendererSchedulerImpl() {
  TRACE_EVENT_OBJECT_DELETED_WITH_ID(
      TRACE_DISABLED_BY_DEFAULT("renderer.scheduler"), "RendererScheduler",
      this);

  for (auto& pair : task_runners_) {
    TaskCostEstimator* observer = nullptr;
    switch (pair.first->queue_class()) {
      case MainThreadTaskQueue::QueueClass::kLoading:
        observer = &main_thread_only().loading_task_cost_estimator;
        break;
      case MainThreadTaskQueue::QueueClass::kTimer:
        observer = &main_thread_only().timer_task_cost_estimator;
        break;
      default:
        observer = nullptr;
    }

    if (observer)
      pair.first->RemoveTaskObserver(observer);
  }

  if (virtual_time_domain_)
    UnregisterTimeDomain(virtual_time_domain_.get());

  base::trace_event::TraceLog::GetInstance()->RemoveAsyncEnabledStateObserver(
      this);

  DCHECK(main_thread_only().was_shutdown);
}
