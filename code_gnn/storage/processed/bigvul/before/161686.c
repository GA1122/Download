void VaapiVideoDecodeAccelerator::NotifyError(Error error) {
  if (!task_runner_->BelongsToCurrentThread()) {
    DCHECK(decoder_thread_task_runner_->BelongsToCurrentThread());
    task_runner_->PostTask(FROM_HERE,
                           base::Bind(&VaapiVideoDecodeAccelerator::NotifyError,
                                      weak_this_, error));
    return;
  }

  task_runner_->PostTask(
      FROM_HERE, base::Bind(&VaapiVideoDecodeAccelerator::Cleanup, weak_this_));

  VLOGF(1) << "Notifying of error " << error;
  if (client_) {
    client_->NotifyError(error);
    client_ptr_factory_.reset();
  }
}
