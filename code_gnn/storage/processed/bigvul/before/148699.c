void V4L2JpegEncodeAccelerator::DestroyTask() {
  DCHECK(encoder_task_runner_->BelongsToCurrentThread());

  while (!encoded_instances_.empty()) {
    encoded_instances_.front()->DestroyTask();
    encoded_instances_.pop();
  }

  while (!encoded_instances_dma_buf_.empty()) {
    encoded_instances_dma_buf_.front()->DestroyTask();
    encoded_instances_dma_buf_.pop();
  }
}
