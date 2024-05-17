blink::WebMediaPlayer::LoadTiming WebMediaPlayerMS::Load(
    LoadType load_type,
    const blink::WebMediaPlayerSource& source,
    CORSMode  ) {
  DVLOG(1) << __func__;
  DCHECK(thread_checker_.CalledOnValidThread());

  DCHECK_NE(load_type, kLoadTypeMediaSource);
  web_stream_ = GetWebMediaStreamFromWebMediaPlayerSource(source);
  if (!web_stream_.IsNull())
    web_stream_.AddObserver(this);

  compositor_ = new WebMediaPlayerMSCompositor(
      compositor_task_runner_, io_task_runner_, web_stream_,
      create_submitter_callback_, surface_layer_for_video_enabled_,
      AsWeakPtr());

  SetNetworkState(WebMediaPlayer::kNetworkStateLoading);
  SetReadyState(WebMediaPlayer::kReadyStateHaveNothing);
  std::string stream_id =
      web_stream_.IsNull() ? std::string() : web_stream_.Id().Utf8();
  media_log_->AddEvent(media_log_->CreateLoadEvent(stream_id));

  frame_deliverer_.reset(new WebMediaPlayerMS::FrameDeliverer(
      AsWeakPtr(),
      base::BindRepeating(&WebMediaPlayerMSCompositor::EnqueueFrame,
                          compositor_),
      media_task_runner_, worker_task_runner_, gpu_factories_));
  video_frame_provider_ = renderer_factory_->GetVideoRenderer(
      web_stream_,
      media::BindToCurrentLoop(
          base::Bind(&WebMediaPlayerMS::OnSourceError, AsWeakPtr())),
      frame_deliverer_->GetRepaintCallback(), io_task_runner_);

  RenderFrame* const frame = RenderFrame::FromWebFrame(frame_);

  int routing_id = MSG_ROUTING_NONE;
  GURL url = source.IsURL() ? GURL(source.GetAsURL()) : GURL();

  if (frame) {
    media::ReportMetrics(load_type, url, *frame_, media_log_.get());
    routing_id = frame->GetRoutingID();
  }

  audio_renderer_ = renderer_factory_->GetAudioRenderer(
      web_stream_, routing_id, initial_audio_output_device_id_);

  if (!audio_renderer_)
    WebRtcLogMessage("Warning: Failed to instantiate audio renderer.");

  if (!video_frame_provider_ && !audio_renderer_) {
    SetNetworkState(WebMediaPlayer::kNetworkStateNetworkError);
    return blink::WebMediaPlayer::LoadTiming::kImmediate;
  }

  if (audio_renderer_) {
    audio_renderer_->SetVolume(volume_);
    audio_renderer_->Start();

    if (!web_stream_.IsNull()) {
      blink::WebVector<blink::WebMediaStreamTrack> audio_tracks =
          web_stream_.AudioTracks();
      DCHECK_GT(audio_tracks.size(), 0U);
      current_audio_track_id_ = audio_tracks[0].Id();
    }
  }

  if (video_frame_provider_) {
    video_frame_provider_->Start();

    if (!web_stream_.IsNull()) {
      blink::WebVector<blink::WebMediaStreamTrack> video_tracks =
          web_stream_.VideoTracks();
      DCHECK_GT(video_tracks.size(), 0U);
      current_video_track_id_ = video_tracks[0].Id();
    }
  }
  if (audio_renderer_ &&
      (client_->IsAudioElement() || !video_frame_provider_)) {
    SetReadyState(WebMediaPlayer::kReadyStateHaveMetadata);
    SetReadyState(WebMediaPlayer::kReadyStateHaveEnoughData);
  }

  return blink::WebMediaPlayer::LoadTiming::kImmediate;
}
