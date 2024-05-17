void RendererSchedulerImpl::OnIdlePeriodEnded() {
  base::AutoLock lock(any_thread_lock_);
  any_thread().last_idle_period_end_time = helper_.NowTicks();
  any_thread().in_idle_period = false;
  UpdatePolicyLocked(UpdateType::kMayEarlyOutIfPolicyUnchanged);
}
