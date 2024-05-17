WebKit::WebUserMediaClient* RenderViewImpl::userMediaClient() {
  const CommandLine* cmd_line = CommandLine::ForCurrentProcess();
  if (!cmd_line->HasSwitch(switches::kEnableMediaStream))
    return NULL;
  EnsureMediaStreamImpl();
  return media_stream_impl_;
}
