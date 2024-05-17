void VaapiVideoDecodeAccelerator::Reset() {
  VLOGF(2) << "Got reset request";
  DCHECK(task_runner_->BelongsToCurrentThread());

  base::AutoLock auto_lock(lock_);
  state_ = kResetting;
  finish_flush_pending_ = false;

  while (!input_buffers_.empty())
    input_buffers_.pop();
  TRACE_COUNTER1("Video Decoder", "Input buffers", input_buffers_.size());

  decoder_thread_task_runner_->PostTask(
      FROM_HERE, base::Bind(&VaapiVideoDecodeAccelerator::ResetTask,
                            base::Unretained(this)));

  input_ready_.Signal();
  surfaces_available_.Signal();
}
