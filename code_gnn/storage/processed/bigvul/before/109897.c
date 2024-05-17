void GpuProcessHost::EstablishGpuChannel(
    int client_id,
    bool share_context,
    const EstablishChannelCallback& callback) {
  DCHECK(CalledOnValidThread());
  TRACE_EVENT0("gpu", "GpuProcessHostUIShim::EstablishGpuChannel");

  if (!GpuDataManagerImpl::GetInstance()->GpuAccessAllowed()) {
    EstablishChannelError(
        callback, IPC::ChannelHandle(),
        base::kNullProcessHandle, content::GPUInfo());
    return;
  }

  if (Send(new GpuMsg_EstablishChannel(client_id, share_context))) {
    channel_requests_.push(callback);
  } else {
    EstablishChannelError(
        callback, IPC::ChannelHandle(),
        base::kNullProcessHandle, content::GPUInfo());
  }
}
