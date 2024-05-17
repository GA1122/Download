WebMediaPlayer* RenderViewImpl::createMediaPlayer(
    WebFrame* frame, WebMediaPlayerClient* client) {
  FOR_EACH_OBSERVER(
      RenderViewObserver, observers_, WillCreateMediaPlayer(frame, client));

  media::MessageLoopFactory* message_loop_factory =
      new media::MessageLoopFactory();
  media::FilterCollection* collection = new media::FilterCollection();
  RenderMediaLog* render_media_log = new RenderMediaLog();

  RenderAudioSourceProvider* audio_source_provider = NULL;

  const CommandLine* cmd_line = CommandLine::ForCurrentProcess();
  if (!cmd_line->HasSwitch(switches::kDisableAudio)) {
    audio_source_provider = new RenderAudioSourceProvider();

    AudioRendererImpl* audio_renderer =
        new AudioRendererImpl(audio_source_provider);
    collection->AddAudioRenderer(audio_renderer);
  }

#if defined(OS_CHROMEOS) && defined(ARCH_CPU_ARMEL)
  WebGraphicsContext3DCommandBufferImpl* context3d =
      static_cast<WebGraphicsContext3DCommandBufferImpl*>(
          webview()->graphicsContext3D());
  if (context_is_web_graphics_context_3d_command_buffer_impl_ && context3d) {
    MessageLoop* factories_loop =
        RenderThreadImpl::current()->compositor_thread() ?
        RenderThreadImpl::current()->compositor_thread()->GetWebThread()
            ->message_loop() :
        MessageLoop::current();
    GpuChannelHost* gpu_channel_host =
        RenderThreadImpl::current()->EstablishGpuChannelSync(
            content::CAUSE_FOR_GPU_LAUNCH_VIDEODECODEACCELERATOR_INITIALIZE);
    collection->AddVideoDecoder(new media::GpuVideoDecoder(
        message_loop_factory->GetMessageLoop("GpuVideoDecoder"),
        factories_loop,
        new RendererGpuVideoDecoderFactories(
            gpu_channel_host, factories_loop, context3d)));
  }
#endif

  webkit_media::WebMediaPlayerImpl* media_player =
      content::GetContentClient()->renderer()->OverrideCreateWebMediaPlayer(
          this, frame, client, AsWeakPtr(), collection, audio_source_provider,
          message_loop_factory, media_stream_impl_.get(), render_media_log);
#if defined(OS_ANDROID)
#else
  if (!media_player) {
    media_player = new webkit_media::WebMediaPlayerImpl(
        frame, client, AsWeakPtr(), collection, audio_source_provider,
        message_loop_factory, media_stream_impl_.get(), render_media_log);
  }
#endif
  return media_player;
}
