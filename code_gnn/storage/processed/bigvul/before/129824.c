void FFmpegVideoDecoder::Stop() {
  DCHECK(task_runner_->BelongsToCurrentThread());

  if (state_ == kUninitialized)
    return;

  ReleaseFFmpegResources();
  state_ = kUninitialized;
}
