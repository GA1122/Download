void WebMediaPlayerMS::Play() {
  DVLOG(1) << __func__;
  DCHECK(thread_checker_.CalledOnValidThread());

  media_log_->AddEvent(media_log_->CreateEvent(media::MediaLogEvent::PLAY));
  if (!paused_)
    return;

  if (video_frame_provider_)
    video_frame_provider_->Resume();

  compositor_->StartRendering();

  if (audio_renderer_)
    audio_renderer_->Play();

  if (HasVideo())
    delegate_->DidPlayerSizeChange(delegate_id_, NaturalSize());

  if (HasAudio() || HasVideo()) {
    delegate_->DidPlay(delegate_id_, HasVideo(), HasAudio(),
                       media::MediaContentType::OneShot);
  }

  delegate_->SetIdle(delegate_id_, false);
  paused_ = false;
}
