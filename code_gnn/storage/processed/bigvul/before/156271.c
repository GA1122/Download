void RendererSchedulerImpl::Shutdown() {
  if (main_thread_only().was_shutdown)
    return;

  base::TimeTicks now = tick_clock()->NowTicks();
  main_thread_only().metrics_helper.OnRendererShutdown(now);

  task_queue_throttler_.reset();
  idle_helper_.Shutdown();
  helper_.Shutdown();
  main_thread_only().was_shutdown = true;
  main_thread_only().rail_mode_observer = nullptr;
}
