void GpuProcessHost::OnChannelEstablished(
    const IPC::ChannelHandle& channel_handle) {
  TRACE_EVENT0("gpu", "GpuProcessHostUIShim::OnChannelEstablished");

  EstablishChannelCallback callback = channel_requests_.front();
  channel_requests_.pop();

  if (!channel_handle.name.empty() &&
      !GpuDataManagerImpl::GetInstance()->GpuAccessAllowed()) {
    Send(new GpuMsg_CloseChannel(channel_handle));
    EstablishChannelError(callback,
                          IPC::ChannelHandle(),
                          base::kNullProcessHandle,
                          GPUInfo());
    RouteOnUIThread(GpuHostMsg_OnLogMessage(
        logging::LOG_WARNING,
        "WARNING",
        "Hardware acceleration is unavailable."));
    return;
  }

  callback.Run(channel_handle,
               GpuDataManagerImpl::GetInstance()->GetGPUInfo());
}
