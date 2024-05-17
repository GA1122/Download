void RenderThreadImpl::Init() {
  TRACE_EVENT_BEGIN_ETW("RenderThreadImpl::Init", 0, "");

#if defined(OS_MACOSX) || defined(OS_ANDROID)
  WebKit::WebView::setUseExternalPopupMenus(true);
#endif

  lazy_tls.Pointer()->Set(this);

#if defined(OS_WIN)
  if (RenderProcessImpl::InProcessPlugins())
    initialize_com_.reset(new base::win::ScopedCOMInitializer());
#endif

  ChildProcess::current()->set_main_thread(this);

  suspend_webkit_shared_timer_ = true;
  notify_webkit_of_modal_loop_ = true;
  plugin_refresh_allowed_ = true;
  widget_count_ = 0;
  hidden_widget_count_ = 0;
  idle_notification_delay_in_ms_ = kInitialIdleHandlerDelayMs;
  idle_notifications_to_skip_ = 0;
  compositor_initialized_ = false;

  appcache_dispatcher_.reset(new AppCacheDispatcher(Get()));
  main_thread_indexed_db_dispatcher_.reset(
      IndexedDBDispatcher::ThreadSpecificInstance());

  media_stream_center_ = NULL;

  db_message_filter_ = new DBMessageFilter();
  AddFilter(db_message_filter_.get());

  vc_manager_ = new VideoCaptureImplManager();
  AddFilter(vc_manager_->video_capture_message_filter());

  audio_input_message_filter_ = new AudioInputMessageFilter();
  AddFilter(audio_input_message_filter_.get());

  audio_message_filter_ = new AudioMessageFilter();
  AddFilter(audio_message_filter_.get());

  devtools_agent_message_filter_ = new DevToolsAgentFilter();
  AddFilter(devtools_agent_message_filter_.get());

  AddFilter(new IndexedDBMessageFilter);

  content::GetContentClient()->renderer()->RenderThreadStarted();

  FilePath media_path;
  PathService::Get(content::DIR_MEDIA_LIBS, &media_path);
  if (!media_path.empty())
    media::InitializeMediaLibrary(media_path);

  TRACE_EVENT_END_ETW("RenderThreadImpl::Init", 0, "");
}
