void RendererSchedulerImpl::EnsureUrgentPolicyUpdatePostedOnMainThread(
    const base::Location& from_here) {
  any_thread_lock_.AssertAcquired();
  if (!policy_may_need_update_.IsSet()) {
    policy_may_need_update_.SetWhileLocked(true);
    control_task_queue_->PostTask(from_here, update_policy_closure_);
  }
}
