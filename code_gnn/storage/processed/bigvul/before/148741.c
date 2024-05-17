void V4L2JpegEncodeAccelerator::ServiceDeviceTaskLegacy() {
  DCHECK(encoder_task_runner_->BelongsToCurrentThread());

  encoded_instances_.front()->ServiceDevice();

  if (encoded_instances_.size() > 1) {
    if (encoded_instances_.front()->running_job_queue_.empty() &&
        encoded_instances_.front()->input_job_queue_.empty()) {
      encoded_instances_.pop();
    }
  }

  if (!encoded_instances_.front()->running_job_queue_.empty() ||
      !encoded_instances_.front()->input_job_queue_.empty()) {
    encoder_task_runner_->PostTask(
        FROM_HERE,
        base::BindOnce(&V4L2JpegEncodeAccelerator::ServiceDeviceTaskLegacy,
                       base::Unretained(this)));
  }
}
