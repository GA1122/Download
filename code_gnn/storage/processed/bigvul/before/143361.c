void Job::CheckIsOnOriginThread() const {
  DCHECK(origin_runner_->BelongsToCurrentThread());
}
