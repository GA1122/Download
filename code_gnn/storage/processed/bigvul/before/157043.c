void MultibufferDataSource::SeekTask() {
  DCHECK(render_task_runner_->BelongsToCurrentThread());
  base::AutoLock auto_lock(lock_);
  SeekTask_Locked();
}
