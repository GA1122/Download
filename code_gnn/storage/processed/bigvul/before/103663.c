void BrowserRenderProcessHost::CreateMessageFilters() {
  scoped_refptr<RenderMessageFilter> render_message_filter(
      new RenderMessageFilter(
          id(),
          PluginService::GetInstance(),
          profile(),
          profile()->GetRequestContextForRenderProcess(id()),
          widget_helper_));
  channel_->AddFilter(render_message_filter);

  ResourceMessageFilter* resource_message_filter = new ResourceMessageFilter(
      id(), ChildProcessInfo::RENDER_PROCESS,
      &profile()->GetResourceContext(),
      new RendererURLRequestContextSelector(profile(), id()),
      content::GetContentClient()->browser()->GetResourceDispatcherHost());

  channel_->AddFilter(resource_message_filter);
  channel_->AddFilter(new AudioInputRendererHost());
  channel_->AddFilter(new AudioRendererHost(&profile()->GetResourceContext()));
  channel_->AddFilter(new VideoCaptureHost());
  channel_->AddFilter(
      new AppCacheDispatcherHost(&profile()->GetResourceContext(), id()));
  channel_->AddFilter(new ClipboardMessageFilter());
  channel_->AddFilter(
      new DOMStorageMessageFilter(id(), profile()->GetWebKitContext()));
  channel_->AddFilter(
      new IndexedDBDispatcherHost(id(), profile()->GetWebKitContext()));
  channel_->AddFilter(
      GeolocationDispatcherHost::New(
          id(), profile()->GetGeolocationPermissionContext()));
  channel_->AddFilter(new GpuMessageFilter(id(), widget_helper_.get()));
  channel_->AddFilter(new media_stream::MediaStreamDispatcherHost(id()));
  channel_->AddFilter(new PepperFileMessageFilter(id(), profile()));
  channel_->AddFilter(
      new PepperMessageFilter(&profile()->GetResourceContext()));
  channel_->AddFilter(new speech_input::SpeechInputDispatcherHost(id()));
  channel_->AddFilter(
      new FileSystemDispatcherHost(&profile()->GetResourceContext()));
  channel_->AddFilter(new device_orientation::MessageFilter());
  channel_->AddFilter(
      new BlobMessageFilter(id(), profile()->GetBlobStorageContext()));
  channel_->AddFilter(new FileUtilitiesMessageFilter(id()));
  channel_->AddFilter(new MimeRegistryMessageFilter());
  channel_->AddFilter(new DatabaseMessageFilter(
      profile()->GetDatabaseTracker()));

  SocketStreamDispatcherHost* socket_stream_dispatcher_host =
      new SocketStreamDispatcherHost(
          new RendererURLRequestContextSelector(profile(), id()),
          &profile()->GetResourceContext());
  channel_->AddFilter(socket_stream_dispatcher_host);

  channel_->AddFilter(
      new WorkerMessageFilter(
          id(),
          &profile()->GetResourceContext(),
          content::GetContentClient()->browser()->GetResourceDispatcherHost(),
          NewCallbackWithReturnValue(
              widget_helper_.get(), &RenderWidgetHelper::GetNextRoutingID)));

#if defined(ENABLE_P2P_APIS)
  channel_->AddFilter(new P2PSocketDispatcherHost());
#endif

  channel_->AddFilter(new TraceMessageFilter());
  channel_->AddFilter(new ResolveProxyMsgHelper(NULL));
  channel_->AddFilter(new QuotaDispatcherHost(
      id(), profile()->GetQuotaManager(),
      content::GetContentClient()->browser()->CreateQuotaPermissionContext()));
}
