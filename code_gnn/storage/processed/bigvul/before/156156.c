void RendererSchedulerImpl::AddVirtualTimeObserver(
    VirtualTimeObserver* observer) {
  main_thread_only().virtual_time_observers.AddObserver(observer);
}
