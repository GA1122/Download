RenderThread::~RenderThread() {
  RemoveFilter(devtools_agent_filter_.get());
  RemoveFilter(db_message_filter_.get());
  db_message_filter_ = NULL;
  if (webkit_client_.get())
    WebKit::shutdown();

  lazy_tls.Pointer()->Set(NULL);

#if defined(OS_WIN)
  PluginChannelBase::CleanupChannels();
  if (RenderProcess::InProcessPlugins())
    CoUninitialize();
#endif
}
