void WebMediaPlayerImpl::Pause() {
  DVLOG(1) << __func__;
  DCHECK(main_task_runner_->BelongsToCurrentThread());

  paused_ = true;

  paused_when_hidden_ = false;

  if (blink::WebUserGestureIndicator::IsProcessingUserGesture(frame_))
    video_locked_when_paused_when_hidden_ = true;

#if defined(OS_ANDROID)   
  if (IsRemote()) {
    cast_impl_.pause();
    return;
  }
#endif

  pipeline_controller_.SetPlaybackRate(0.0);
  paused_time_ = pipeline_controller_.GetMediaTime();

  if (observer_)
    observer_->OnPaused();

  DCHECK(watch_time_reporter_);
  watch_time_reporter_->OnPaused();

  if (video_decode_stats_reporter_)
    video_decode_stats_reporter_->OnPaused();

  media_log_->AddEvent(media_log_->CreateEvent(MediaLogEvent::PAUSE));

  UpdatePlayState();
}
