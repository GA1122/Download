void WebMediaPlayerImpl::EnableVideoTrackIfNeeded() {
  if (!pipeline_controller_.IsPipelineRunning() || is_pipeline_resuming_ ||
      seeking_)
    return;

  if (video_track_disabled_) {
    video_track_disabled_ = false;
    if (client_->HasSelectedVideoTrack()) {
      WebMediaPlayer::TrackId trackId = client_->GetSelectedVideoTrackId();
      SelectedVideoTrackChanged(&trackId);
    }
  }
}
