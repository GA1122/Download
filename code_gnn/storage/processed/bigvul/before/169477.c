bool NetworkThrottleManagerImpl::ConditionallyTriggerTimerForTesting() {
  if (!outstanding_recomputation_timer_->IsRunning() ||
      (tick_clock_->NowTicks() <
       outstanding_recomputation_timer_->desired_run_time())) {
    return false;
  }

  base::Closure timer_callback(outstanding_recomputation_timer_->user_task());
  outstanding_recomputation_timer_->Stop();
  timer_callback.Run();
  return true;
}
