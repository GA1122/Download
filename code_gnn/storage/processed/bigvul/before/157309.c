void WebMediaPlayerImpl::Seek(double seconds) {
  DVLOG(1) << __func__ << "(" << seconds << "s)";
  DCHECK(main_task_runner_->BelongsToCurrentThread());
  media_log_->AddEvent(media_log_->CreateSeekEvent(seconds));
  DoSeek(base::TimeDelta::FromSecondsD(seconds), true);
}
