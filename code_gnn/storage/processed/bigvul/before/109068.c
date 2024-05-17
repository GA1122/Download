WebMediaPlayer* RenderViewImpl::createMediaPlayer(
    WebFrame* frame, const WebKit::WebURL& url, WebMediaPlayerClient* client) {
  FOR_EACH_OBSERVER(
      RenderViewObserver, observers_, WillCreateMediaPlayer(frame, client));

  const CommandLine* cmd_line = CommandLine::ForCurrentProcess();
#if defined(OS_ANDROID)
  WebGraphicsContext3D* resource_context =
      GetWebView()->sharedGraphicsContext3D();

  GpuChannelHost* gpu_channel_host =
      RenderThreadImpl::current()->EstablishGpuChannelSync(
          content::CAUSE_FOR_GPU_LAUNCH_VIDEODECODEACCELERATOR_INITIALIZE);
  if (!gpu_channel_host) {
    LOG(ERROR) << "Failed to establish GPU channel for media player";
    return NULL;
  }

  if (cmd_line->HasSwitch(switches::kMediaPlayerInRenderProcess)) {
    if (!media_bridge_manager_.get()) {
      media_bridge_manager_.reset(
          new webkit_media::MediaPlayerBridgeManagerImpl(1));
    }
    return new webkit_media::WebMediaPlayerInProcessAndroid(
        frame,
        client,
        cookieJar(frame),
        media_player_manager_.get(),
        media_bridge_manager_.get(),
        new content::StreamTextureFactoryImpl(
            resource_context, gpu_channel_host, routing_id_),
        cmd_line->HasSwitch(switches::kDisableMediaHistoryLogging));
  }
  if (!media_player_proxy_) {
    media_player_proxy_ = new content::WebMediaPlayerProxyImplAndroid(
        this, media_player_manager_.get());
  }
  return new webkit_media::WebMediaPlayerImplAndroid(
      frame,
      client,
      media_player_manager_.get(),
      media_player_proxy_,
      new content::StreamTextureFactoryImpl(
          resource_context, gpu_channel_host, routing_id_));
#endif

  media::MessageLoopFactory* message_loop_factory =
      new media::MessageLoopFactory();
  media::FilterCollection* collection = new media::FilterCollection();
  RenderMediaLog* render_media_log = new RenderMediaLog();

  RenderAudioSourceProvider* audio_source_provider = NULL;

  if (!cmd_line->HasSwitch(switches::kDisableAudio)) {
    audio_source_provider = new RenderAudioSourceProvider();

    media::AudioRendererImpl* audio_renderer =
        new media::AudioRendererImpl(audio_source_provider);
    collection->AddAudioRenderer(audio_renderer);
  }

  WebGraphicsContext3DCommandBufferImpl* context3d = NULL;
  if (!cmd_line->HasSwitch(switches::kDisableAcceleratedVideoDecode))
    context3d = RenderThreadImpl::current()->GetGpuVDAContext3D();
  if (context3d) {
    scoped_refptr<base::MessageLoopProxy> factories_loop =
        RenderThreadImpl::current()->compositor_thread() ?
        RenderThreadImpl::current()->compositor_thread()->GetWebThread()
            ->message_loop()->message_loop_proxy() :
        base::MessageLoopProxy::current();
    GpuChannelHost* gpu_channel_host =
        RenderThreadImpl::current()->EstablishGpuChannelSync(
            content::CAUSE_FOR_GPU_LAUNCH_VIDEODECODEACCELERATOR_INITIALIZE);
    collection->GetVideoDecoders()->push_back(new media::GpuVideoDecoder(
        base::Bind(&media::MessageLoopFactory::GetMessageLoop,
                   base::Unretained(message_loop_factory),
                   media::MessageLoopFactory::kDecoder),
        factories_loop,
        new RendererGpuVideoDecoderFactories(
            gpu_channel_host, factories_loop, context3d)));
  }

  WebMediaPlayer* media_player =
      content::GetContentClient()->renderer()->OverrideCreateWebMediaPlayer(
          this, frame, client, AsWeakPtr(), collection, audio_source_provider,
          audio_source_provider, message_loop_factory, media_stream_impl_,
          render_media_log);
  if (!media_player) {
    if (cmd_line->HasSwitch(switches::kEnableWebMediaPlayerMS)) {
      EnsureMediaStreamImpl();
      if (media_stream_impl_ && media_stream_impl_->IsMediaStream(url)) {
        return new webkit_media::WebMediaPlayerMS(
            frame, client, AsWeakPtr(), media_stream_impl_, render_media_log);
      }
    }

    media_player = new webkit_media::WebMediaPlayerImpl(
        frame, client, AsWeakPtr(), collection, audio_source_provider,
        audio_source_provider, message_loop_factory, media_stream_impl_,
        render_media_log);
  }
  return media_player;
}
