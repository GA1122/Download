WebKit::WebPeerConnectionHandler* RenderViewImpl::CreatePeerConnectionHandler(
    WebKit::WebPeerConnectionHandlerClient* client) {
  const CommandLine* cmd_line = CommandLine::ForCurrentProcess();
  if (!cmd_line->HasSwitch(switches::kEnableMediaStream))
    return NULL;
  EnsureMediaStreamImpl();
  if (!media_stream_impl_.get())
    return NULL;
  return media_stream_impl_->CreatePeerConnectionHandler(client);
}
