VaapiVideoDecodeAccelerator::~VaapiVideoDecodeAccelerator() {
  DCHECK(task_runner_->BelongsToCurrentThread());
}
