void V4L2JpegEncodeAccelerator::VideoFrameReady(int32_t task_id,
                                                size_t encoded_picture_size) {
  if (!child_task_runner_->BelongsToCurrentThread()) {
    child_task_runner_->PostTask(
        FROM_HERE, base::BindOnce(&V4L2JpegEncodeAccelerator::VideoFrameReady,
                                  weak_ptr_, task_id, encoded_picture_size));
    return;
  }
  VLOGF(1) << "Encoding finished task id=" << task_id
           << " Compressed size:" << encoded_picture_size;
  client_->VideoFrameReady(task_id, encoded_picture_size);
}
