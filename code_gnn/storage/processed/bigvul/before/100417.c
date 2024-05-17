void RenderThread::Init() {
  lazy_tls.Pointer()->Set(this);
#if defined(OS_WIN)
  if (RenderProcess::InProcessPlugins())
    CoInitialize(0);
#endif

  plugin_refresh_allowed_ = true;
  cache_stats_task_pending_ = false;
  widget_count_ = 0;
  hidden_widget_count_ = 0;
  idle_notification_delay_in_s_ = kInitialIdleHandlerDelayS;
  task_factory_.reset(new ScopedRunnableMethodFactory<RenderThread>(this));

  visited_link_slave_.reset(new VisitedLinkSlave());
  user_script_slave_.reset(new UserScriptSlave());
  dns_master_.reset(new RenderDnsMaster());
  histogram_snapshots_.reset(new RendererHistogramSnapshots());
  appcache_dispatcher_.reset(new AppCacheDispatcher(this));
  socket_stream_dispatcher_.reset(new SocketStreamDispatcher());
  devtools_agent_filter_ = new DevToolsAgentFilter();
  AddFilter(devtools_agent_filter_.get());
  db_message_filter_ = new DBMessageFilter();
  AddFilter(db_message_filter_.get());

#if defined(OS_POSIX)
  suicide_on_channel_error_filter_ = new SuicideOnChannelErrorFilter;
  AddFilter(suicide_on_channel_error_filter_.get());
#endif
}
