void WebContentsImpl::SendToAllFrames(IPC::Message* message) {
  ForEachFrame(base::Bind(&SendToAllFramesInternal, message));
  delete message;
}
