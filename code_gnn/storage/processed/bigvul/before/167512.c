void WebMediaPlayerImpl::CreateWatchTimeReporter() {
  if (!HasVideo() && !HasAudio())
    return;

  watch_time_reporter_.reset(new WatchTimeReporter(
      mojom::PlaybackProperties::New(pipeline_metadata_.has_audio,
                                     pipeline_metadata_.has_video, false, false,
                                     !!chunk_demuxer_, is_encrypted_,
                                     embedded_media_experience_enabled_),
      pipeline_metadata_.natural_size,
      base::BindRepeating(&WebMediaPlayerImpl::GetCurrentTimeInternal,
                          base::Unretained(this)),
      media_metrics_provider_.get(),
      frame_->GetTaskRunner(blink::TaskType::kInternalMedia)));
  watch_time_reporter_->OnVolumeChange(volume_);

  if (delegate_->IsFrameHidden())
    watch_time_reporter_->OnHidden();
  else
    watch_time_reporter_->OnShown();

  if (client_->HasNativeControls())
    watch_time_reporter_->OnNativeControlsEnabled();
  else
    watch_time_reporter_->OnNativeControlsDisabled();

  switch (client_->DisplayType()) {
    case WebMediaPlayer::DisplayType::kInline:
      watch_time_reporter_->OnDisplayTypeInline();
      break;
    case WebMediaPlayer::DisplayType::kFullscreen:
      watch_time_reporter_->OnDisplayTypeFullscreen();
      break;
    case WebMediaPlayer::DisplayType::kPictureInPicture:
      watch_time_reporter_->OnDisplayTypePictureInPicture();
      break;
  }

  UpdateSecondaryProperties();
}
