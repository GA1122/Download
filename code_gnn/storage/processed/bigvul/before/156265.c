void RendererSchedulerImpl::SetStoppingWhenBackgroundedEnabled(bool enabled) {
  main_thread_only().stopping_when_backgrounded_enabled = enabled;
}
