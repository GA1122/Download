void RendererSchedulerImpl::DecrementVirtualTimePauseCount() {
  main_thread_only().virtual_time_pause_count--;
  DCHECK_GE(main_thread_only().virtual_time_pause_count, 0);
  ApplyVirtualTimePolicy();
}
