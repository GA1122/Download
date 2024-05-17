base::TimeTicks RendererSchedulerImpl::IncrementVirtualTimePauseCount() {
  main_thread_only().virtual_time_pause_count++;
  ApplyVirtualTimePolicy();

  if (virtual_time_domain_)
    return virtual_time_domain_->Now();
  return tick_clock()->NowTicks();
}
