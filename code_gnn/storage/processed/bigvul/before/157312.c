void WebMediaPlayerImpl::SelectedVideoTrackChanged(
    blink::WebMediaPlayer::TrackId* selectedTrackId) {
  DCHECK(main_task_runner_->BelongsToCurrentThread());

  base::Optional<MediaTrack::Id> selected_video_track_id;
  if (selectedTrackId && !video_track_disabled_)
    selected_video_track_id = MediaTrack::Id(selectedTrackId->Utf8().data());
  MEDIA_LOG(INFO, media_log_.get())
      << "Selected video track: [" << selected_video_track_id.value_or("")
      << "]";
  pipeline_controller_.OnSelectedVideoTrackChanged(selected_video_track_id);
}
