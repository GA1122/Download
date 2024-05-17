bool RenderFrameImpl::Send(IPC::Message* message) {
  if (is_detaching_) {
    delete message;
    return false;
  }
  if (is_swapped_out_) {
    if (!SwappedOutMessages::CanSendWhileSwappedOut(message)) {
      delete message;
      return false;
    }
  }

  return RenderThread::Get()->Send(message);
}
