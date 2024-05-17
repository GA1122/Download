void WebMediaPlayerImpl::RecordVideoNaturalSize(const gfx::Size& natural_size) {
  media_log_->AddEvent(media_log_->CreateVideoSizeSetEvent(
      natural_size.width(), natural_size.height()));

  if (initial_video_height_recorded_)
    return;

  initial_video_height_recorded_ = true;

  int height = natural_size.height();

  if (load_type_ == kLoadTypeURL)
    UMA_HISTOGRAM_VIDEO_HEIGHT("Media.VideoHeight.Initial.SRC", height);
  else if (load_type_ == kLoadTypeMediaSource)
    UMA_HISTOGRAM_VIDEO_HEIGHT("Media.VideoHeight.Initial.MSE", height);

  if (is_encrypted_)
    UMA_HISTOGRAM_VIDEO_HEIGHT("Media.VideoHeight.Initial.EME", height);

  UMA_HISTOGRAM_VIDEO_HEIGHT("Media.VideoHeight.Initial.All", height);
}
