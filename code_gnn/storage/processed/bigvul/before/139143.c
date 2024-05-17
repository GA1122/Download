void RenderProcessHostImpl::EnableSendQueue() {
  if (!channel_)
    InitializeChannelProxy();
}
