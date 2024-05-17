void WebMediaPlayerMS::ReloadVideo() {
  DCHECK(thread_checker_.CalledOnValidThread());
  DCHECK(!web_stream_.IsNull());
  blink::WebVector<blink::WebMediaStreamTrack> video_tracks =
      web_stream_.VideoTracks();

  RendererReloadAction renderer_action = RendererReloadAction::KEEP_RENDERER;
  if (video_tracks.IsEmpty()) {
    if (video_frame_provider_)
      renderer_action = RendererReloadAction::REMOVE_RENDERER;
    current_video_track_id_ = blink::WebString();
  } else if (video_tracks[0].Id() != current_video_track_id_ &&
             IsPlayableTrack(video_tracks[0])) {
    renderer_action = RendererReloadAction::NEW_RENDERER;
    current_video_track_id_ = video_tracks[0].Id();
  }

  switch (renderer_action) {
    case RendererReloadAction::NEW_RENDERER:
      if (video_frame_provider_)
        video_frame_provider_->Stop();

      SetNetworkState(kNetworkStateLoading);
      video_frame_provider_ = renderer_factory_->GetVideoRenderer(
          web_stream_,
          media::BindToCurrentLoop(
              base::Bind(&WebMediaPlayerMS::OnSourceError, AsWeakPtr())),
          frame_deliverer_->GetRepaintCallback(), io_task_runner_);
      DCHECK(video_frame_provider_);
      video_frame_provider_->Start();
      break;

    case RendererReloadAction::REMOVE_RENDERER:
      video_frame_provider_->Stop();
      video_frame_provider_ = nullptr;
      break;

    default:
      return;
  }

  DCHECK_NE(renderer_action, RendererReloadAction::KEEP_RENDERER);
  if (!paused_)
    delegate_->DidPlayerSizeChange(delegate_id_, NaturalSize());
}
