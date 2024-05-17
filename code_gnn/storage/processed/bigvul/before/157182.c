void WebMediaPlayerImpl::CreateVideoDecodeStatsReporter() {
  if (!HasVideo())
    return;

  if (!pipeline_metadata_.video_decoder_config.IsValidConfig())
    return;

  if (is_encrypted_)
    return;

  mojom::VideoDecodeStatsRecorderPtr recorder;
  media_metrics_provider_->AcquireVideoDecodeStatsRecorder(
      mojo::MakeRequest(&recorder));

  video_decode_stats_reporter_.reset(new VideoDecodeStatsReporter(
      std::move(recorder),
      base::Bind(&WebMediaPlayerImpl::GetPipelineStatistics,
                 base::Unretained(this)),
      pipeline_metadata_.video_decoder_config,
      frame_->GetTaskRunner(blink::TaskType::kInternalMedia)));

  if (delegate_->IsFrameHidden())
    video_decode_stats_reporter_->OnHidden();
  else
    video_decode_stats_reporter_->OnShown();

  if (paused_)
    video_decode_stats_reporter_->OnPaused();
  else
    video_decode_stats_reporter_->OnPlaying();
}
