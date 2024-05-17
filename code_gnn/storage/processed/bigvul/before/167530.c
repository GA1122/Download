void WebMediaPlayerImpl::OnMetadata(PipelineMetadata metadata) {
  DVLOG(1) << __func__;
  DCHECK(main_task_runner_->BelongsToCurrentThread());

  time_to_metadata_ = base::TimeTicks::Now() - load_start_time_;
  media_metrics_provider_->SetTimeToMetadata(time_to_metadata_);
  RecordTimingUMA("Media.TimeToMetadata", time_to_metadata_);

  pipeline_metadata_ = metadata;

  SetReadyState(WebMediaPlayer::kReadyStateHaveMetadata);
  UMA_HISTOGRAM_ENUMERATION("Media.VideoRotation",
                            metadata.video_decoder_config.video_rotation(),
                            VIDEO_ROTATION_MAX + 1);

  if (HasAudio()) {
    RecordEncryptionScheme("Audio",
                           metadata.audio_decoder_config.encryption_scheme());
  }

  if (HasVideo()) {
    RecordEncryptionScheme("Video",
                           metadata.video_decoder_config.encryption_scheme());

    if (overlay_enabled_) {
      if (!always_enable_overlays_ && !DoesOverlaySupportMetadata())
        DisableOverlay();
    }

    if (!surface_layer_for_video_enabled_) {
      DCHECK(!video_layer_);
      video_layer_ = cc::VideoLayer::Create(
          compositor_.get(),
          pipeline_metadata_.video_decoder_config.video_rotation());
      video_layer_->SetContentsOpaque(opaque_);
      client_->SetCcLayer(video_layer_.get());
    } else {
      DCHECK(!bridge_);

      bridge_ = std::move(create_bridge_callback_)
                    .Run(this, compositor_->GetUpdateSubmissionStateCallback());
      bridge_->CreateSurfaceLayer();

      vfc_task_runner_->PostTask(
          FROM_HERE,
          base::BindOnce(
              &VideoFrameCompositor::EnableSubmission,
              base::Unretained(compositor_.get()), bridge_->GetSurfaceId(),
              pipeline_metadata_.video_decoder_config.video_rotation(),
              BindToCurrentLoop(base::BindRepeating(
                  &WebMediaPlayerImpl::OnFrameSinkDestroyed, AsWeakPtr()))));
      bridge_->SetContentsOpaque(opaque_);
    }
  }

  if (observer_)
    observer_->OnMetadataChanged(pipeline_metadata_);

  CreateWatchTimeReporter();
  CreateVideoDecodeStatsReporter();

  UpdatePlayState();
}
