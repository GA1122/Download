bool RendererSchedulerImpl::IsVirualTimeEnabled() const {
  return main_thread_only().use_virtual_time;
}
