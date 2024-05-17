void WebMediaPlayerMS::ReloadAudio() {
  DCHECK(thread_checker_.CalledOnValidThread());
  DCHECK(!web_stream_.IsNull());
  RenderFrame* const frame = RenderFrame::FromWebFrame(frame_);
  if (!frame)
    return;

  blink::WebVector<blink::WebMediaStreamTrack> audio_tracks =
      web_stream_.AudioTracks();

  RendererReloadAction renderer_action = RendererReloadAction::KEEP_RENDERER;
  if (audio_tracks.IsEmpty()) {
    if (audio_renderer_)
      renderer_action = RendererReloadAction::REMOVE_RENDERER;
    current_audio_track_id_ = blink::WebString();
  } else if (audio_tracks[0].Id() != current_audio_track_id_ &&
             IsPlayableTrack(audio_tracks[0])) {
    renderer_action = RendererReloadAction::NEW_RENDERER;
    current_audio_track_id_ = audio_tracks[0].Id();
  }

  switch (renderer_action) {
    case RendererReloadAction::NEW_RENDERER:
      if (audio_renderer_)
        audio_renderer_->Stop();

      SetNetworkState(WebMediaPlayer::kNetworkStateLoading);
      audio_renderer_ = renderer_factory_->GetAudioRenderer(
          web_stream_, frame->GetRoutingID(), initial_audio_output_device_id_);

      if (!audio_renderer_)
        break;

      audio_renderer_->SetVolume(volume_);
      audio_renderer_->Start();
      audio_renderer_->Play();
      break;

    case RendererReloadAction::REMOVE_RENDERER:
      audio_renderer_->Stop();
      audio_renderer_ = nullptr;
      break;

    default:
      break;
  }
}
