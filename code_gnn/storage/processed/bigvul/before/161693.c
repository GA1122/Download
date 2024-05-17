void VaapiVideoDecodeAccelerator::ResetTask() {
  VLOGF(2);
  DCHECK(decoder_thread_task_runner_->BelongsToCurrentThread());

  decoder_->Reset();

  base::AutoLock auto_lock(lock_);

  if (curr_input_buffer_.get())
    ReturnCurrInputBuffer_Locked();

  task_runner_->PostTask(
      FROM_HERE,
      base::Bind(&VaapiVideoDecodeAccelerator::FinishReset, weak_this_));
}
