void WebMediaPlayerImpl::DoSeek(base::TimeDelta time, bool time_updated) {
  DCHECK(main_task_runner_->BelongsToCurrentThread());
  TRACE_EVENT2("media", "WebMediaPlayerImpl::DoSeek", "target",
               time.InSecondsF(), "id", media_log_->id());

#if defined(OS_ANDROID)   
  if (IsRemote()) {
    cast_impl_.seek(time);
    return;
  }
#endif

  ReadyState old_state = ready_state_;
  if (ready_state_ > WebMediaPlayer::kReadyStateHaveMetadata)
    SetReadyState(WebMediaPlayer::kReadyStateHaveMetadata);

  if (paused_ && pipeline_controller_.IsStable() &&
      (paused_time_ == time ||
       (ended_ && time == base::TimeDelta::FromSecondsD(Duration()))) &&
      !chunk_demuxer_) {
    if (old_state == kReadyStateHaveEnoughData) {
      main_task_runner_->PostTask(
          FROM_HERE, base::Bind(&WebMediaPlayerImpl::OnBufferingStateChange,
                                AsWeakPtr(), BUFFERING_HAVE_ENOUGH));
    }
    return;
  }

  if (watch_time_reporter_)
    watch_time_reporter_->OnSeeking();

  frame_time_report_cb_.Cancel();

  delegate_->SetIdle(delegate_id_, false);
  ended_ = false;
  seeking_ = true;
  seek_time_ = time;
  if (paused_)
    paused_time_ = time;
  pipeline_controller_.Seek(time, time_updated);

  UpdatePlayState();
}
