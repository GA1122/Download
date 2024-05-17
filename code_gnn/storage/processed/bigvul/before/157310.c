blink::WebTimeRanges WebMediaPlayerImpl::Seekable() const {
  DCHECK(main_task_runner_->BelongsToCurrentThread());

  if (ready_state_ < WebMediaPlayer::kReadyStateHaveMetadata)
    return blink::WebTimeRanges();

  const double seekable_end = Duration();

  const bool is_finite_stream = data_source_ && data_source_->IsStreaming() &&
                                std::isfinite(seekable_end);

  const bool force_seeks_to_zero =
      !using_media_player_renderer_ && is_finite_stream;

  const blink::WebTimeRange seekable_range(
      0.0, force_seeks_to_zero ? 0.0 : seekable_end);
  return blink::WebTimeRanges(&seekable_range, 1);
}
