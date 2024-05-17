void RendererSchedulerImpl::SetVirtualTimePolicy(VirtualTimePolicy policy) {
  main_thread_only().virtual_time_policy = policy;
  ApplyVirtualTimePolicy();
}
