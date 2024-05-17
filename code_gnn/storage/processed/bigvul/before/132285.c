blink::WebMediaPlayer* RenderFrameImpl::createMediaPlayer(
    blink::WebLocalFrame* frame,
    const blink::WebURL& url,
    WebMediaPlayerClient* client,
    WebMediaPlayerEncryptedMediaClient* encrypted_client,
    WebContentDecryptionModule* initial_cdm) {
#if defined(VIDEO_HOLE)
  if (!contains_media_player_) {
    render_view_->RegisterVideoHoleFrame(this);
    contains_media_player_ = true;
  }
#endif   

  blink::WebMediaStream web_stream(
      blink::WebMediaStreamRegistry::lookupMediaStreamDescriptor(url));
  if (!web_stream.isNull())
    return CreateWebMediaPlayerForMediaStream(client);

#if defined(OS_ANDROID) && !defined(ENABLE_MEDIA_PIPELINE_ON_ANDROID)
  return CreateAndroidWebMediaPlayer(client, encrypted_client,
                                     GetMediaPermission(), initial_cdm);
#else
  scoped_refptr<media::MediaLog> media_log(new RenderMediaLog());

  RenderThreadImpl* render_thread = RenderThreadImpl::current();
  media::WebMediaPlayerParams params(
      base::Bind(&ContentRendererClient::DeferMediaLoad,
                 base::Unretained(GetContentClient()->renderer()),
                 static_cast<RenderFrame*>(this), has_played_media_),
      render_thread->GetAudioRendererMixerManager()->CreateInput(routing_id_),
      media_log, render_thread->GetMediaThreadTaskRunner(),
      render_thread->GetWorkerTaskRunner(),
      render_thread->compositor_task_runner(),
      base::Bind(&GetSharedMainThreadContext3D), GetMediaPermission(),
      initial_cdm);

#if defined(ENABLE_MOJO_MEDIA)
  scoped_ptr<media::RendererFactory> media_renderer_factory(
      new media::MojoRendererFactory(GetMediaServiceFactory()));
#else
  scoped_ptr<media::RendererFactory> media_renderer_factory =
      GetContentClient()->renderer()->CreateMediaRendererFactory(
          this, render_thread->GetGpuFactories(), media_log);

  if (!media_renderer_factory.get()) {
    media_renderer_factory.reset(new media::DefaultRendererFactory(
        media_log, render_thread->GetGpuFactories(),
        *render_thread->GetAudioHardwareConfig()));
  }
#endif   

  return new media::WebMediaPlayerImpl(
      frame, client, encrypted_client, weak_factory_.GetWeakPtr(),
      media_renderer_factory.Pass(), GetCdmFactory(), params);
#endif   
}
