scoped_refptr<MainThreadTaskQueue> RendererSchedulerImpl::NewTaskQueue(
    const MainThreadTaskQueue::QueueCreationParams& params) {
  helper_.CheckOnValidThread();
  scoped_refptr<MainThreadTaskQueue> task_queue(helper_.NewTaskQueue(params));

  std::unique_ptr<TaskQueue::QueueEnabledVoter> voter;
  if (params.can_be_blocked || params.can_be_paused || params.can_be_stopped)
    voter = task_queue->CreateQueueEnabledVoter();

  auto insert_result =
      task_runners_.insert(std::make_pair(task_queue, std::move(voter)));
  auto queue_class = task_queue->queue_class();
  if (queue_class == MainThreadTaskQueue::QueueClass::kTimer) {
    task_queue->AddTaskObserver(&main_thread_only().timer_task_cost_estimator);
  } else if (queue_class == MainThreadTaskQueue::QueueClass::kLoading) {
    task_queue->AddTaskObserver(
        &main_thread_only().loading_task_cost_estimator);
  }

  ApplyTaskQueuePolicy(
      task_queue.get(), insert_result.first->second.get(), TaskQueuePolicy(),
      main_thread_only().current_policy.GetQueuePolicy(queue_class));

  if (task_queue->CanBeThrottled())
    AddQueueToWakeUpBudgetPool(task_queue.get());

  if (queue_class == MainThreadTaskQueue::QueueClass::kTimer) {
    if (main_thread_only().virtual_time_stopped)
      task_queue->InsertFence(TaskQueue::InsertFencePosition::kNow);
  }

  return task_queue;
}
