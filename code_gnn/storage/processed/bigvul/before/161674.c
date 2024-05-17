void VaapiVideoDecodeAccelerator::FinishFlush() {
  VLOGF(2);
  DCHECK(task_runner_->BelongsToCurrentThread());

  finish_flush_pending_ = false;

  base::AutoLock auto_lock(lock_);
  if (state_ != kDecoding) {
    DCHECK(state_ == kDestroying || state_ == kResetting) << state_;
    return;
  }

  if (!pending_output_cbs_.empty()) {
    finish_flush_pending_ = true;
    return;
  }

  if (input_buffers_.empty()) {
    state_ = kIdle;
  } else {
    decoder_thread_task_runner_->PostTask(
        FROM_HERE, base::Bind(&VaapiVideoDecodeAccelerator::DecodeTask,
                              base::Unretained(this)));
  }

  task_runner_->PostTask(FROM_HERE,
                         base::Bind(&Client::NotifyFlushDone, client_));
}
