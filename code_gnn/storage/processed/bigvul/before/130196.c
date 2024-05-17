bool RenderFrameHostImpl::Send(IPC::Message* message) {
  return GetProcess()->Send(message);
 }
