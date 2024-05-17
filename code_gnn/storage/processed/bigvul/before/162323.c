MojoJpegDecodeAccelerator::~MojoJpegDecodeAccelerator() {
  DCHECK(io_task_runner_->BelongsToCurrentThread());
}
