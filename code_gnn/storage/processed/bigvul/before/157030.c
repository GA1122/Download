bool MultibufferDataSource::HasSingleOrigin() {
  DCHECK(render_task_runner_->BelongsToCurrentThread());
   return single_origin_;
 }
