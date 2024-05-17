V4L2JpegEncodeAccelerator::~V4L2JpegEncodeAccelerator() {
  DCHECK(child_task_runner_->BelongsToCurrentThread());

  if (encoder_thread_.IsRunning()) {
    encoder_task_runner_->PostTask(
        FROM_HERE, base::BindOnce(&V4L2JpegEncodeAccelerator::DestroyTask,
                                  base::Unretained(this)));
    encoder_thread_.Stop();
  }
  weak_factory_.InvalidateWeakPtrs();
}
