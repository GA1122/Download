void WebMediaPlayerImpl::OnDisplayTypeChanged(
    WebMediaPlayer::DisplayType display_type) {
  if (!watch_time_reporter_)
    return;

  switch (display_type) {
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
}
