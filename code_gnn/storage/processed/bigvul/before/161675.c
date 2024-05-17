void VaapiVideoDecodeAccelerator::FinishReset() {
  VLOGF(2);
  DCHECK(task_runner_->BelongsToCurrentThread());
  base::AutoLock auto_lock(lock_);

  if (state_ != kResetting) {
    DCHECK(state_ == kDestroying || state_ == kUninitialized) << state_;
    return;   
  }

  while (!pending_output_cbs_.empty())
    pending_output_cbs_.pop();

  if (awaiting_va_surfaces_recycle_) {
    task_runner_->PostTask(
        FROM_HERE,
        base::Bind(&VaapiVideoDecodeAccelerator::FinishReset, weak_this_));
    return;
  }

  state_ = kIdle;

  task_runner_->PostTask(FROM_HERE,
                         base::Bind(&Client::NotifyResetDone, client_));

  if (!input_buffers_.empty()) {
    state_ = kDecoding;
    decoder_thread_task_runner_->PostTask(
        FROM_HERE, base::Bind(&VaapiVideoDecodeAccelerator::DecodeTask,
                              base::Unretained(this)));
  }
}
