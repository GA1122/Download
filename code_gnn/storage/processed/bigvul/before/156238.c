void RendererSchedulerImpl::OnVirtualTimeAdvanced() {
  for (auto& observer : main_thread_only().virtual_time_observers) {
    observer.OnVirtualTimeAdvanced(virtual_time_domain_->Now() -
                                   main_thread_only().initial_virtual_time);
  }
}
