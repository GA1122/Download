void VaapiVideoDecodeAccelerator::Destroy() {
  DCHECK(task_runner_->BelongsToCurrentThread());
  Cleanup();
  delete this;
}
