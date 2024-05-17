void VaapiVideoDecodeAccelerator::NotifyError(Error error) {
  if (message_loop_ != base::MessageLoop::current()) {
    DCHECK(decoder_thread_task_runner_->BelongsToCurrentThread());
    message_loop_->PostTask(FROM_HERE, base::Bind(
        &VaapiVideoDecodeAccelerator::NotifyError, weak_this_, error));
    return;
  }

  message_loop_->PostTask(FROM_HERE, base::Bind(
      &VaapiVideoDecodeAccelerator::Cleanup, weak_this_));

  LOG(ERROR) << "Notifying of error " << error;
  if (client_) {
    client_->NotifyError(error);
    client_ptr_factory_.reset();
  }
}
