void WebMediaPlayerImpl::OnFrameShown() {
  DCHECK(main_task_runner_->BelongsToCurrentThread());
  background_pause_timer_.Stop();

  video_locked_when_paused_when_hidden_ = false;

  if (watch_time_reporter_)
    watch_time_reporter_->OnShown();

  if (video_decode_stats_reporter_)
    video_decode_stats_reporter_->OnShown();

  if ((!paused_ && IsBackgroundOptimizationCandidate()) ||
      paused_when_hidden_) {
    frame_time_report_cb_.Reset(base::BindOnce(
        &WebMediaPlayerImpl::ReportTimeFromForegroundToFirstFrame, AsWeakPtr(),
        base::TimeTicks::Now()));
    vfc_task_runner_->PostTask(
        FROM_HERE,
        base::BindOnce(&VideoFrameCompositor::SetOnNewProcessedFrameCallback,
                       base::Unretained(compositor_.get()),
                       BindToCurrentLoop(frame_time_report_cb_.callback())));
  }

  UpdateBackgroundVideoOptimizationState();

  if (paused_when_hidden_) {
    paused_when_hidden_ = false;
    OnPlay();   
    return;
  }

  UpdatePlayState();
}
