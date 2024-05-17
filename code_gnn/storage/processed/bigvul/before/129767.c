void ChildThread::Init() {
  g_lazy_tls.Pointer()->Set(this);
  on_channel_error_called_ = false;
  message_loop_ = base::MessageLoop::current();
#ifdef IPC_MESSAGE_LOG_ENABLED
  IPC::Logging::GetInstance();
#endif
  channel_.reset(
      new IPC::SyncChannel(channel_name_,
                           IPC::Channel::MODE_CLIENT,
                           this,
                           ChildProcess::current()->io_message_loop_proxy(),
                           true,
                           ChildProcess::current()->GetShutDownEvent()));
#ifdef IPC_MESSAGE_LOG_ENABLED
  if (!in_browser_process_)
    IPC::Logging::GetInstance()->SetIPCSender(this);
#endif

  mojo_application_.reset(new MojoApplication(this));

  sync_message_filter_ =
      new IPC::SyncMessageFilter(ChildProcess::current()->GetShutDownEvent());
  thread_safe_sender_ = new ThreadSafeSender(
      base::MessageLoopProxy::current().get(), sync_message_filter_.get());

  resource_dispatcher_.reset(new ResourceDispatcher(this));
  socket_stream_dispatcher_.reset(new SocketStreamDispatcher());
  websocket_dispatcher_.reset(new WebSocketDispatcher);
  file_system_dispatcher_.reset(new FileSystemDispatcher());

  histogram_message_filter_ = new ChildHistogramMessageFilter();
  resource_message_filter_ =
      new ChildResourceMessageFilter(resource_dispatcher());

  service_worker_message_filter_ =
      new ServiceWorkerMessageFilter(thread_safe_sender_.get());
  service_worker_dispatcher_.reset(
      new ServiceWorkerDispatcher(thread_safe_sender_.get()));

  quota_message_filter_ =
      new QuotaMessageFilter(thread_safe_sender_.get());
  quota_dispatcher_.reset(new QuotaDispatcher(thread_safe_sender_.get(),
                                              quota_message_filter_.get()));

  channel_->AddFilter(histogram_message_filter_.get());
  channel_->AddFilter(sync_message_filter_.get());
  channel_->AddFilter(new tracing::ChildTraceMessageFilter(
      ChildProcess::current()->io_message_loop_proxy()));
  channel_->AddFilter(resource_message_filter_.get());
  channel_->AddFilter(quota_message_filter_->GetFilter());
  channel_->AddFilter(service_worker_message_filter_->GetFilter());

  if (!base::PowerMonitor::Get()) {
    scoped_ptr<PowerMonitorBroadcastSource> power_monitor_source(
      new PowerMonitorBroadcastSource());
    channel_->AddFilter(power_monitor_source->GetMessageFilter());

    power_monitor_.reset(new base::PowerMonitor(
        power_monitor_source.PassAs<base::PowerMonitorSource>()));
  }

#if defined(OS_POSIX)
  if (CommandLine::ForCurrentProcess()->HasSwitch(switches::kProcessType))
    channel_->AddFilter(new SuicideOnChannelErrorFilter());
#endif

  base::MessageLoop::current()->PostDelayedTask(
      FROM_HERE,
      base::Bind(&ChildThread::EnsureConnected,
                 channel_connected_factory_.GetWeakPtr()),
      base::TimeDelta::FromSeconds(kConnectionTimeoutS));

#if defined(OS_ANDROID)
  {
    base::AutoLock lock(g_lazy_child_thread_lock.Get());
    g_child_thread = this;
  }
  g_lazy_child_thread_cv.Get().Signal();
#endif

#if defined(TCMALLOC_TRACE_MEMORY_SUPPORTED)
  trace_memory_controller_.reset(new base::debug::TraceMemoryController(
      message_loop_->message_loop_proxy(),
      ::HeapProfilerWithPseudoStackStart,
      ::HeapProfilerStop,
      ::GetHeapProfile));
#endif

  shared_bitmap_manager_.reset(
      new ChildSharedBitmapManager(thread_safe_sender()));
}
