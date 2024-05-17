WebMediaPlayerImpl::WebMediaPlayerImpl(
    blink::WebLocalFrame* frame,
    blink::WebMediaPlayerClient* client,
    blink::WebMediaPlayerEncryptedMediaClient* encrypted_client,
    WebMediaPlayerDelegate* delegate,
    std::unique_ptr<RendererFactorySelector> renderer_factory_selector,
    UrlIndex* url_index,
    std::unique_ptr<VideoFrameCompositor> compositor,
    std::unique_ptr<WebMediaPlayerParams> params)
    : frame_(frame),
      main_task_runner_(
          frame->GetTaskRunner(blink::TaskType::kMediaElementEvent)),
      media_task_runner_(params->media_task_runner()),
      worker_task_runner_(params->worker_task_runner()),
      media_log_(params->take_media_log()),
      pipeline_controller_(
          std::make_unique<PipelineImpl>(media_task_runner_,
                                         main_task_runner_,
                                         media_log_.get()),
          base::Bind(&WebMediaPlayerImpl::CreateRenderer,
                     base::Unretained(this)),
          base::Bind(&WebMediaPlayerImpl::OnPipelineSeeked, AsWeakPtr()),
          base::Bind(&WebMediaPlayerImpl::OnPipelineSuspended, AsWeakPtr()),
          base::Bind(&WebMediaPlayerImpl::OnBeforePipelineResume, AsWeakPtr()),
          base::Bind(&WebMediaPlayerImpl::OnPipelineResumed, AsWeakPtr()),
          base::Bind(&WebMediaPlayerImpl::OnError, AsWeakPtr())),
      client_(client),
      encrypted_client_(encrypted_client),
      delegate_(delegate),
      defer_load_cb_(params->defer_load_cb()),
      adjust_allocated_memory_cb_(params->adjust_allocated_memory_cb()),
      tick_clock_(base::DefaultTickClock::GetInstance()),
      buffered_data_source_host_(
          base::Bind(&WebMediaPlayerImpl::OnProgress, AsWeakPtr()),
          tick_clock_),
      url_index_(url_index),
      context_provider_(params->context_provider()),
      vfc_task_runner_(params->video_frame_compositor_task_runner()),
      compositor_(std::move(compositor)),
#if defined(OS_ANDROID)   
      cast_impl_(this, client_, params->context_provider()),
#endif
      renderer_factory_selector_(std::move(renderer_factory_selector)),
      observer_(params->media_observer()),
      max_keyframe_distance_to_disable_background_video_(
          params->max_keyframe_distance_to_disable_background_video()),
      max_keyframe_distance_to_disable_background_video_mse_(
          params->max_keyframe_distance_to_disable_background_video_mse()),
      enable_instant_source_buffer_gc_(
          params->enable_instant_source_buffer_gc()),
      embedded_media_experience_enabled_(
          params->embedded_media_experience_enabled()),
      surface_layer_for_video_enabled_(params->use_surface_layer_for_video()),
      create_bridge_callback_(params->create_bridge_callback()),
      request_routing_token_cb_(params->request_routing_token_cb()),
      overlay_routing_token_(OverlayInfo::RoutingToken()),
      media_metrics_provider_(params->take_metrics_provider()) {
  DVLOG(1) << __func__;
  DCHECK(!adjust_allocated_memory_cb_.is_null());
  DCHECK(renderer_factory_selector_);
  DCHECK(client_);
  DCHECK(delegate_);

  always_enable_overlays_ = base::CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kForceVideoOverlays);

  if (base::FeatureList::IsEnabled(media::kOverlayFullscreenVideo)) {
    bool use_android_overlay =
        base::FeatureList::IsEnabled(media::kUseAndroidOverlay);
    overlay_mode_ = use_android_overlay ? OverlayMode::kUseAndroidOverlay
                                        : OverlayMode::kUseContentVideoView;
  } else {
    overlay_mode_ = OverlayMode::kNoOverlays;
  }

  delegate_id_ = delegate_->AddObserver(this);
  delegate_->SetIdle(delegate_id_, true);

  media_log_->AddEvent(media_log_->CreateCreatedEvent(
      url::Origin(frame_->GetSecurityOrigin()).GetURL().spec()));
  media_log_->SetStringProperty("frame_url",
                                frame_->GetDocument().Url().GetString().Utf8());
  media_log_->SetStringProperty("frame_title",
                                frame_->GetDocument().Title().Utf8());

  if (params->initial_cdm())
    SetCdm(params->initial_cdm());

  RecordEncryptedEvent(false);

  audio_source_provider_ = new WebAudioSourceProviderImpl(
      params->audio_renderer_sink(), media_log_.get());

  if (observer_)
    observer_->SetClient(this);

  memory_usage_reporting_timer_.SetTaskRunner(
      frame_->GetTaskRunner(blink::TaskType::kInternalMedia));
}
