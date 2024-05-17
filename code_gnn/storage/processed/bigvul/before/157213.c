base::TimeDelta WebMediaPlayerImpl::GetCurrentTimeInternal() const {
  DCHECK(main_task_runner_->BelongsToCurrentThread());
  DCHECK_NE(ready_state_, WebMediaPlayer::kReadyStateHaveNothing);

  base::TimeDelta current_time;
  if (Seeking())
    current_time = seek_time_;
#if defined(OS_ANDROID)   
  else if (IsRemote())
    current_time = cast_impl_.currentTime();
#endif
  else if (paused_)
    current_time = paused_time_;
  else
    current_time = pipeline_controller_.GetMediaTime();

  DCHECK_NE(current_time, kInfiniteDuration);
  DCHECK_GE(current_time, base::TimeDelta());
  return current_time;
}
