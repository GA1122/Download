void WebMediaPlayerImpl::EnabledAudioTracksChanged(
    const blink::WebVector<blink::WebMediaPlayer::TrackId>& enabledTrackIds) {
  DCHECK(main_task_runner_->BelongsToCurrentThread());

  std::ostringstream logstr;
  std::vector<MediaTrack::Id> enabledMediaTrackIds;
  for (const auto& blinkTrackId : enabledTrackIds) {
    MediaTrack::Id track_id = blinkTrackId.Utf8().data();
    logstr << track_id << " ";
    enabledMediaTrackIds.push_back(track_id);
  }
  MEDIA_LOG(INFO, media_log_.get())
      << "Enabled audio tracks: [" << logstr.str() << "]";
  pipeline_controller_.OnEnabledAudioTracksChanged(enabledMediaTrackIds);
}
