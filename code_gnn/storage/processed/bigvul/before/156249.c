void RendererSchedulerImpl::RemoveVirtualTimeObserver(
    VirtualTimeObserver* observer) {
  main_thread_only().virtual_time_observers.RemoveObserver(observer);
}
