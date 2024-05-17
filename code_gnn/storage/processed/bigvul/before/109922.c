void GpuProcessHost::SendOutstandingReplies() {
  while (!channel_requests_.empty()) {
    EstablishChannelCallback callback = channel_requests_.front();
    channel_requests_.pop();
    EstablishChannelError(callback,
                          IPC::ChannelHandle(),
                          base::kNullProcessHandle,
                          content::GPUInfo());
  }
}
