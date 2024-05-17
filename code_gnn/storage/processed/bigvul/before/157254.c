void WebMediaPlayerImpl::OnDisplayTypeChanged(
    WebMediaPlayer::DisplayType display_type) {
  if (surface_layer_for_video_enabled_) {
    vfc_task_runner_->PostTask(
        FROM_HERE,
        base::BindOnce(
            &VideoFrameCompositor::SetForceSubmit,
            base::Unretained(compositor_.get()),
            display_type == WebMediaPlayer::DisplayType::kPictureInPicture));
  }

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
