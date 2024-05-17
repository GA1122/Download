void V4L2JpegEncodeAccelerator::ServiceDeviceTask() {
  DCHECK(encoder_task_runner_->BelongsToCurrentThread());

  encoded_instances_dma_buf_.front()->ServiceDevice();

  if (encoded_instances_dma_buf_.size() > 1) {
    if (encoded_instances_dma_buf_.front()->running_job_queue_.empty() &&
        encoded_instances_dma_buf_.front()->input_job_queue_.empty()) {
      encoded_instances_dma_buf_.pop();
    }
  }

  if (!encoded_instances_dma_buf_.front()->running_job_queue_.empty() ||
      !encoded_instances_dma_buf_.front()->input_job_queue_.empty()) {
    encoder_task_runner_->PostTask(
        FROM_HERE, base::BindOnce(&V4L2JpegEncodeAccelerator::ServiceDeviceTask,
                                  base::Unretained(this)));
  }
}
