WebMediaPlayer* RenderFrameImpl::CreateAndroidWebMediaPlayer(
    WebMediaPlayerClient* client,
    WebMediaPlayerEncryptedMediaClient* encrypted_client,
    media::MediaPermission* media_permission,
    WebContentDecryptionModule* initial_cdm) {
  scoped_refptr<StreamTextureFactory> stream_texture_factory;
  if (SynchronousCompositorFactory* factory =
          SynchronousCompositorFactory::GetInstance()) {
    stream_texture_factory = factory->CreateStreamTextureFactory(routing_id_);
  } else {
    GpuChannelHost* gpu_channel_host =
        RenderThreadImpl::current()->EstablishGpuChannelSync(
            CAUSE_FOR_GPU_LAUNCH_VIDEODECODEACCELERATOR_INITIALIZE);

    if (!gpu_channel_host) {
      LOG(ERROR) << "Failed to establish GPU channel for media player";
      return NULL;
    }

    scoped_refptr<cc_blink::ContextProviderWebContext> context_provider =
        RenderThreadImpl::current()->SharedMainThreadContextProvider();

    if (!context_provider.get()) {
      LOG(ERROR) << "Failed to get context3d for media player";
      return NULL;
    }

    stream_texture_factory = StreamTextureFactoryImpl::Create(
        context_provider, gpu_channel_host, routing_id_);
  }

  return new WebMediaPlayerAndroid(
      frame_, client, encrypted_client, weak_factory_.GetWeakPtr(),
      GetMediaPlayerManager(), GetCdmFactory(), media_permission, initial_cdm,
      stream_texture_factory,
      RenderThreadImpl::current()->GetMediaThreadTaskRunner(),
      new RenderMediaLog());
}
