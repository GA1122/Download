void WebMediaPlayerImpl::OnPipelineSeeked(bool time_updated) {
  TRACE_EVENT2("media", "WebMediaPlayerImpl::OnPipelineSeeked", "target",
               seek_time_.InSecondsF(), "id", media_log_->id());
  seeking_ = false;
  seek_time_ = base::TimeDelta();

  if (paused_) {
#if defined(OS_ANDROID)   
    if (IsRemote()) {
      paused_time_ = cast_impl_.currentTime();
    } else {
      paused_time_ = pipeline_controller_.GetMediaTime();
    }
#else
    paused_time_ = pipeline_controller_.GetMediaTime();
#endif
  } else {
    DCHECK(watch_time_reporter_);
    watch_time_reporter_->OnPlaying();
  }
  if (time_updated)
    should_notify_time_changed_ = true;

  underflow_timer_.reset();

  UpdateBackgroundVideoOptimizationState();

  if (attempting_suspended_start_ &&
      pipeline_controller_.IsPipelineSuspended()) {
    skip_metrics_due_to_startup_suspend_ = true;
    OnBufferingStateChangeInternal(BUFFERING_HAVE_ENOUGH, true);

    base::UmaHistogramBoolean(
        std::string("Media.PreloadMetadataSuspendWasIdeal.") +
            ((HasVideo() && HasAudio()) ? "AudioVideo"
                                        : (HasVideo() ? "Video" : "Audio")),
        skip_metrics_due_to_startup_suspend_);
  }

  attempting_suspended_start_ = false;
}
