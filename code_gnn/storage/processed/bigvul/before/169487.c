void NetworkThrottleManagerImpl::ThrottleImpl::SetPriority(
    RequestPriority new_priority) {
  RequestPriority old_priority(priority_);
  if (old_priority == new_priority)
    return;
  priority_ = new_priority;
  manager_->OnThrottlePriorityChanged(this, old_priority, new_priority);
}
