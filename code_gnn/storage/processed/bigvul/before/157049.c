void MultibufferDataSource::StopLoader() {
  DCHECK(render_task_runner_->BelongsToCurrentThread());
  SetReader(nullptr);
}
