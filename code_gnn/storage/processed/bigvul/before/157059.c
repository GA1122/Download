MultibufferDataSource::~MultibufferDataSource() {
  DCHECK(render_task_runner_->BelongsToCurrentThread());
}
