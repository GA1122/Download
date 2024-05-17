void WebMediaPlayerImpl::OnVideoConfigChange(const VideoDecoderConfig& config) {
  DCHECK(main_task_runner_->BelongsToCurrentThread());
  DCHECK_NE(ready_state_, WebMediaPlayer::kReadyStateHaveNothing);

  const bool codec_change =
      pipeline_metadata_.video_decoder_config.codec() != config.codec();

  pipeline_metadata_.video_decoder_config = config;

  if (observer_)
    observer_->OnMetadataChanged(pipeline_metadata_);

  if (video_decode_stats_reporter_)
    video_decode_stats_reporter_->OnVideoConfigChanged(config);

  if (codec_change)
    UpdateSecondaryProperties();
}
