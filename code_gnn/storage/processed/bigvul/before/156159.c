void RendererSchedulerImpl::ApplyTaskQueuePolicy(
    MainThreadTaskQueue* task_queue,
    TaskQueue::QueueEnabledVoter* task_queue_enabled_voter,
    const TaskQueuePolicy& old_task_queue_policy,
    const TaskQueuePolicy& new_task_queue_policy) const {
  DCHECK(old_task_queue_policy.IsQueueEnabled(task_queue) ||
         task_queue_enabled_voter);
  if (task_queue_enabled_voter) {
    task_queue_enabled_voter->SetQueueEnabled(
        new_task_queue_policy.IsQueueEnabled(task_queue));
  }

  DCHECK(task_queue_enabled_voter ||
         old_task_queue_policy.IsQueueEnabled(task_queue));

  task_queue->SetQueuePriority(new_task_queue_policy.GetPriority(task_queue));

  TimeDomainType old_time_domain_type =
      old_task_queue_policy.GetTimeDomainType(task_queue);
  TimeDomainType new_time_domain_type =
      new_task_queue_policy.GetTimeDomainType(task_queue);

  if (old_time_domain_type != new_time_domain_type) {
    if (old_time_domain_type == TimeDomainType::kThrottled) {
      task_queue_throttler_->DecreaseThrottleRefCount(task_queue);
    } else if (new_time_domain_type == TimeDomainType::kThrottled) {
      task_queue_throttler_->IncreaseThrottleRefCount(task_queue);
    }
    if (new_time_domain_type == TimeDomainType::kVirtual) {
      DCHECK(virtual_time_domain_);
      task_queue->SetTimeDomain(virtual_time_domain_.get());
    } else {
      task_queue->SetTimeDomain(real_time_domain());
    }
  }
}
