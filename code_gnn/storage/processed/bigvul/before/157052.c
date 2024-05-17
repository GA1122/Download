void MultibufferDataSource::UpdateProgress() {
  DCHECK(render_task_runner_->BelongsToCurrentThread());
  if (reader_) {
    uint64_t available = reader_->Available();
    uint64_t pos = reader_->Tell();
    ProgressCallback(pos, pos + available);
  }
}
