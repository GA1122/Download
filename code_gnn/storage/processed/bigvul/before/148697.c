void V4L2JpegEncodeAccelerator::EncodedInstance::DestroyTask() {
  DCHECK(parent_->encoder_task_runner_->BelongsToCurrentThread());
  while (!input_job_queue_.empty())
    input_job_queue_.pop();
  while (!running_job_queue_.empty())
    running_job_queue_.pop();

  DestroyInputBuffers();
  DestroyOutputBuffers();
}
