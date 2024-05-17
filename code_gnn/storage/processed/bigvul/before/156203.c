RendererSchedulerImpl::TaskQueuePolicy::GetTimeDomainType(
    MainThreadTaskQueue* task_queue) const {
  if (use_virtual_time)
    return TimeDomainType::kVirtual;
  if (is_throttled && task_queue->CanBeThrottled())
    return TimeDomainType::kThrottled;
  return TimeDomainType::kReal;
}
