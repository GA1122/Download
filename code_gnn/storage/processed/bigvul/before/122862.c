void RenderProcessHostImpl::CreateMessageFilters() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  MediaObserver* media_observer =
      GetContentClient()->browser()->GetMediaObserver();
  if (IsGuest()) {
    scoped_refptr<BrowserPluginMessageFilter> bp_message_filter(
        new BrowserPluginMessageFilter(
            GetID(),
            GetBrowserContext()));
    channel_->AddFilter(bp_message_filter);
  }

  scoped_refptr<RenderMessageFilter> render_message_filter(
      new RenderMessageFilter(
          GetID(),
          PluginServiceImpl::GetInstance(),
          GetBrowserContext(),
          GetBrowserContext()->GetRequestContextForRenderProcess(GetID()),
          widget_helper_,
          media_observer,
          storage_partition_impl_->GetDOMStorageContext()));
  channel_->AddFilter(render_message_filter);
  BrowserContext* browser_context = GetBrowserContext();
  ResourceContext* resource_context = browser_context->GetResourceContext();

  ResourceMessageFilter* resource_message_filter = new ResourceMessageFilter(
      GetID(), PROCESS_TYPE_RENDERER, resource_context,
      storage_partition_impl_->GetAppCacheService(),
      ChromeBlobStorageContext::GetFor(browser_context),
      new RendererURLRequestContextSelector(browser_context, GetID()));

  channel_->AddFilter(resource_message_filter);
  media::AudioManager* audio_manager = BrowserMainLoop::GetAudioManager();
  MediaStreamManager* media_stream_manager =
      BrowserMainLoop::GetMediaStreamManager();
  channel_->AddFilter(new AudioInputRendererHost(audio_manager,
                                                 media_stream_manager));
  channel_->AddFilter(new AudioRendererHost(audio_manager, media_observer));
  channel_->AddFilter(new VideoCaptureHost());
  channel_->AddFilter(new AppCacheDispatcherHost(
      storage_partition_impl_->GetAppCacheService(),
      GetID()));
  channel_->AddFilter(new ClipboardMessageFilter());
  channel_->AddFilter(
      new DOMStorageMessageFilter(
          GetID(),
          storage_partition_impl_->GetDOMStorageContext()));
  channel_->AddFilter(
      new IndexedDBDispatcherHost(
          GetID(),
          storage_partition_impl_->GetIndexedDBContext()));
  channel_->AddFilter(GeolocationDispatcherHost::New(
      GetID(), browser_context->GetGeolocationPermissionContext()));
  gpu_message_filter_ = new GpuMessageFilter(GetID(), widget_helper_.get());
  channel_->AddFilter(gpu_message_filter_);
#if defined(ENABLE_WEBRTC)
  channel_->AddFilter(new MediaStreamDispatcherHost(GetID()));
#endif
#if defined(ENABLE_PLUGINS)
  channel_->AddFilter(
      GetContentClient()->browser()->AllowPepperPrivateFileAPI() ?
          new PepperUnsafeFileMessageFilter(
              GetID(),
              storage_partition_impl_->GetPath()) :
          new PepperFileMessageFilter(GetID()));
  channel_->AddFilter(new PepperMessageFilter(PepperMessageFilter::RENDERER,
                                              GetID(), browser_context));
#endif
#if defined(ENABLE_INPUT_SPEECH)
  channel_->AddFilter(new InputTagSpeechDispatcherHost(
      IsGuest(), GetID(), storage_partition_impl_->GetURLRequestContext(),
      browser_context->GetSpeechRecognitionPreferences()));
  channel_->AddFilter(new SpeechRecognitionDispatcherHost(
      GetID(), storage_partition_impl_->GetURLRequestContext(),
      browser_context->GetSpeechRecognitionPreferences()));
#endif
  channel_->AddFilter(new FileAPIMessageFilter(
      GetID(),
      storage_partition_impl_->GetURLRequestContext(),
      storage_partition_impl_->GetFileSystemContext(),
      ChromeBlobStorageContext::GetFor(browser_context)));
  channel_->AddFilter(new OrientationMessageFilter());
  channel_->AddFilter(new FileUtilitiesMessageFilter(GetID()));
  channel_->AddFilter(new MimeRegistryMessageFilter());
  channel_->AddFilter(new DatabaseMessageFilter(
      storage_partition_impl_->GetDatabaseTracker()));
#if defined(OS_MACOSX)
  channel_->AddFilter(new TextInputClientMessageFilter(GetID()));
#elif defined(OS_WIN)
  channel_->AddFilter(new FontCacheDispatcher());
#endif

  SocketStreamDispatcherHost* socket_stream_dispatcher_host =
      new SocketStreamDispatcherHost(GetID(),
          new RendererURLRequestContextSelector(browser_context, GetID()),
          resource_context);
  channel_->AddFilter(socket_stream_dispatcher_host);

  channel_->AddFilter(
      new WorkerMessageFilter(
          GetID(),
          resource_context,
          WorkerStoragePartition(
              storage_partition_impl_->GetURLRequestContext(),
              storage_partition_impl_->GetMediaURLRequestContext(),
              storage_partition_impl_->GetAppCacheService(),
              storage_partition_impl_->GetFileSystemContext(),
              storage_partition_impl_->GetDatabaseTracker(),
              storage_partition_impl_->GetIndexedDBContext()),
          base::Bind(&RenderWidgetHelper::GetNextRoutingID,
                     base::Unretained(widget_helper_.get()))));

#if defined(ENABLE_WEBRTC)
  channel_->AddFilter(new P2PSocketDispatcherHost(resource_context));
#endif

  channel_->AddFilter(new TraceMessageFilter());
  channel_->AddFilter(new ResolveProxyMsgHelper(
      browser_context->GetRequestContextForRenderProcess(GetID())));
  channel_->AddFilter(new QuotaDispatcherHost(
      GetID(),
      storage_partition_impl_->GetQuotaManager(),
      GetContentClient()->browser()->CreateQuotaPermissionContext()));
  channel_->AddFilter(new GamepadBrowserMessageFilter());
  channel_->AddFilter(new ProfilerMessageFilter(PROCESS_TYPE_RENDERER));
  channel_->AddFilter(new HistogramMessageFilter());
  channel_->AddFilter(new HyphenatorMessageFilter(this));
}
