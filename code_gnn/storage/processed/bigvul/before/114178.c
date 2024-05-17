BrowserGpuChannelHostFactory::BrowserGpuChannelHostFactory()
    : gpu_client_id_(ChildProcessHostImpl::GenerateChildProcessUniqueId()),
      shutdown_event_(new base::WaitableEvent(true, false)),
      gpu_host_id_(0) {
}
