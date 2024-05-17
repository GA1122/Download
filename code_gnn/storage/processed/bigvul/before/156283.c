bool RendererSchedulerImpl::VirtualTimeAllowedToAdvance() const {
  return !main_thread_only().virtual_time_stopped;
}
