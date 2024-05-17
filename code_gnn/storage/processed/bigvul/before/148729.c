void V4L2JpegEncodeAccelerator::NotifyError(int32_t task_id, Status status) {
  if (!child_task_runner_->BelongsToCurrentThread()) {
    child_task_runner_->PostTask(
        FROM_HERE, base::BindOnce(&V4L2JpegEncodeAccelerator::NotifyError,
                                  weak_ptr_, task_id, status));

    return;
  }
  VLOGF(1) << "Notifying of error " << status << " for task id " << task_id;
  client_->NotifyError(task_id, status);
}
