void MultibufferDataSource::SetReader(MultiBufferReader* reader) {
  DCHECK(render_task_runner_->BelongsToCurrentThread());
  base::AutoLock auto_lock(lock_);
  reader_.reset(reader);
}
