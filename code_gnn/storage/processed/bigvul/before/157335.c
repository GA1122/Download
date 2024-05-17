void WebMediaPlayerImpl::StartPipeline() {
  DCHECK(main_task_runner_->BelongsToCurrentThread());

  Demuxer::EncryptedMediaInitDataCB encrypted_media_init_data_cb =
      BindToCurrentLoop(base::Bind(
          &WebMediaPlayerImpl::OnEncryptedMediaInitData, AsWeakPtr()));

  vfc_task_runner_->PostTask(
      FROM_HERE,
      base::BindOnce(&VideoFrameCompositor::SetOnNewProcessedFrameCallback,
                     base::Unretained(compositor_.get()),
                     BindToCurrentLoop(base::BindOnce(
                         &WebMediaPlayerImpl::OnFirstFrame, AsWeakPtr()))));

  if (renderer_factory_selector_->GetCurrentFactory()
          ->GetRequiredMediaResourceType() == MediaResource::Type::URL) {
    if (data_source_)
      loaded_url_ = data_source_->GetUrlAfterRedirects();

    using_media_player_renderer_ = true;

    video_decode_stats_reporter_.reset();

    demuxer_.reset(new MediaUrlDemuxer(media_task_runner_, loaded_url_,
                                       frame_->GetDocument().SiteForCookies()));
    pipeline_controller_.Start(Pipeline::StartType::kNormal, demuxer_.get(),
                               this, false, false);
    return;
  }

  if (load_type_ != kLoadTypeMediaSource) {
    DCHECK(!chunk_demuxer_);
    DCHECK(data_source_);

#if BUILDFLAG(ENABLE_FFMPEG)
    Demuxer::MediaTracksUpdatedCB media_tracks_updated_cb =
        BindToCurrentLoop(base::Bind(
            &WebMediaPlayerImpl::OnFFmpegMediaTracksUpdated, AsWeakPtr()));

    demuxer_.reset(new FFmpegDemuxer(
        media_task_runner_, data_source_.get(), encrypted_media_init_data_cb,
        media_tracks_updated_cb, media_log_.get(), IsLocalFile(loaded_url_)));
#else
    OnError(PipelineStatus::DEMUXER_ERROR_COULD_NOT_OPEN);
    return;
#endif
  } else {
    DCHECK(!chunk_demuxer_);
    DCHECK(!data_source_);

    chunk_demuxer_ = new ChunkDemuxer(
        BindToCurrentLoop(
            base::Bind(&WebMediaPlayerImpl::OnDemuxerOpened, AsWeakPtr())),
        BindToCurrentLoop(
            base::Bind(&WebMediaPlayerImpl::OnProgress, AsWeakPtr())),
        encrypted_media_init_data_cb, media_log_.get());
    demuxer_.reset(chunk_demuxer_);

    if (base::FeatureList::IsEnabled(kMemoryPressureBasedSourceBufferGC)) {
      memory_pressure_listener_ =
          std::make_unique<base::MemoryPressureListener>(base::Bind(
              &WebMediaPlayerImpl::OnMemoryPressure, base::Unretained(this)));
    }
  }

  bool is_static = !chunk_demuxer_;
  bool is_streaming = IsStreaming();
  UMA_HISTOGRAM_BOOLEAN("Media.IsStreaming", is_streaming);

  Pipeline::StartType start_type = Pipeline::StartType::kNormal;
  if (!chunk_demuxer_ && preload_ == MultibufferDataSource::METADATA &&
      !client_->CouldPlayIfEnoughData()) {
    start_type =
        (has_poster_ || base::FeatureList::IsEnabled(kPreloadMetadataLazyLoad))
            ? Pipeline::StartType::kSuspendAfterMetadata
            : Pipeline::StartType::kSuspendAfterMetadataForAudioOnly;
    attempting_suspended_start_ = true;
  }

  seeking_ = true;
  pipeline_controller_.Start(start_type, demuxer_.get(), this, is_streaming,
                             is_static);
}
