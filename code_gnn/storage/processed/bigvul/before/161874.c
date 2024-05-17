void DiscardableSharedMemoryManager::ScheduleEnforceMemoryPolicy() {
  lock_.AssertAcquired();

  if (enforce_memory_policy_pending_)
    return;

  enforce_memory_policy_pending_ = true;
  DCHECK(enforce_memory_policy_task_runner_);
  enforce_memory_policy_task_runner_->PostDelayedTask(
      FROM_HERE, enforce_memory_policy_callback_,
      base::TimeDelta::FromMilliseconds(kEnforceMemoryPolicyDelayMs));
}
