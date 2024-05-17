void MultibufferDataSource::SetBitrateTask(int bitrate) {
  DCHECK(render_task_runner_->BelongsToCurrentThread());

  bitrate_ = bitrate;
  UpdateBufferSizes();
}
