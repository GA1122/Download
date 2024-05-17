void WebMediaPlayerImpl::OnBufferingStateChangeInternal(
    BufferingState state,
    bool for_suspended_start) {
  DVLOG(1) << __func__ << "(" << state << ")";
  DCHECK(main_task_runner_->BelongsToCurrentThread());

  if (!pipeline_controller_.IsStable() && !for_suspended_start)
    return;

  auto log_event = media_log_->CreateBufferingStateChangedEvent(
      "pipeline_buffering_state", state);
  log_event->params.SetBoolean("for_suspended_start", for_suspended_start);
  media_log_->AddEvent(std::move(log_event));

  if (state == BUFFERING_HAVE_ENOUGH) {
    TRACE_EVENT1("media", "WebMediaPlayerImpl::BufferingHaveEnough", "id",
                 media_log_->id());
    if (!have_reported_time_to_play_ready_ &&
        !skip_metrics_due_to_startup_suspend_) {
      DCHECK(!for_suspended_start);
      have_reported_time_to_play_ready_ = true;
      const base::TimeDelta elapsed = base::TimeTicks::Now() - load_start_time_;
      media_metrics_provider_->SetTimeToPlayReady(elapsed);
      RecordTimingUMA("Media.TimeToPlayReady", elapsed);
    }

    SetReadyState(CanPlayThrough() ? WebMediaPlayer::kReadyStateHaveEnoughData
                                   : WebMediaPlayer::kReadyStateHaveFutureData);

    if (data_source_ && !client_->CouldPlayIfEnoughData())
      data_source_->OnBufferingHaveEnough(false);

    if (should_notify_time_changed_) {
      should_notify_time_changed_ = false;
      client_->TimeChanged();
    }

    ReportMemoryUsage();

    if (underflow_timer_) {
      RecordUnderflowDuration(underflow_timer_->Elapsed());
      underflow_timer_.reset();
    }
  } else {
    DCHECK_EQ(state, BUFFERING_HAVE_NOTHING);

    if (ready_state_ == WebMediaPlayer::kReadyStateHaveEnoughData &&
        !seeking_) {
      underflow_timer_.reset(new base::ElapsedTimer());
      watch_time_reporter_->OnUnderflow();
    }

    DCHECK_GT(highest_ready_state_, WebMediaPlayer::kReadyStateHaveCurrentData);
    SetReadyState(WebMediaPlayer::kReadyStateHaveCurrentData);
  }

  UpdatePlayState();
}
