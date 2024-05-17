WebMediaPlayerImpl::~WebMediaPlayerImpl() {
  DVLOG(1) << __func__;
  DCHECK(main_task_runner_->BelongsToCurrentThread());

  if (set_cdm_result_) {
    DVLOG(2) << "Resolve pending SetCdm() when media player is destroyed.";
    set_cdm_result_->Complete();
    set_cdm_result_.reset();
  }

  suppress_destruction_errors_ = true;

  delegate_->PlayerGone(delegate_id_);
  delegate_->RemoveObserver(delegate_id_);

  watch_time_reporter_.reset();

  pipeline_controller_.Stop();

  if (last_reported_memory_usage_)
    adjust_allocated_memory_cb_.Run(-last_reported_memory_usage_);

  client_->SetCcLayer(nullptr);

  client_->MediaRemotingStopped(
      blink::WebLocalizedString::kMediaRemotingStopNoText);

  if (!surface_layer_for_video_enabled_ && video_layer_) {
    video_layer_->StopUsingProvider();
  }

  vfc_task_runner_->DeleteSoon(FROM_HERE, std::move(compositor_));

  if (chunk_demuxer_) {
    media_task_runner_->PostTask(
        FROM_HERE, base::BindOnce(&WebMediaPlayerImpl::DemuxerDestructionHelper,
                                  media_task_runner_, std::move(demuxer_)));
  }

  media_log_->AddEvent(
      media_log_->CreateEvent(MediaLogEvent::WEBMEDIAPLAYER_DESTROYED));
}
