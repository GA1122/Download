bool RenderFrameImpl::Send(IPC::Message* message) {
  return RenderThread::Get()->Send(message);
}
