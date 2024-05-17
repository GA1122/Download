void RendererSchedulerImpl::SetRAILModeObserver(RAILModeObserver* observer) {
  main_thread_only().rail_mode_observer = observer;
}
