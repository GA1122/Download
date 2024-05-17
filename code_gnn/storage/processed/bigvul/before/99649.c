void VaapiVideoDecodeAccelerator::Reset() {
  DCHECK_EQ(message_loop_, base::MessageLoop::current());
  DVLOG(1) << "Got reset request";

  base::AutoLock auto_lock(lock_);
  state_ = kResetting;
  finish_flush_pending_ = false;

  while (!input_buffers_.empty()) {
    message_loop_->PostTask(FROM_HERE, base::Bind(
        &Client::NotifyEndOfBitstreamBuffer, client_,
        input_buffers_.front()->id));
    input_buffers_.pop();
  }

  decoder_thread_task_runner_->PostTask(
      FROM_HERE, base::Bind(&VaapiVideoDecodeAccelerator::ResetTask,
                            base::Unretained(this)));

  input_ready_.Signal();
  surfaces_available_.Signal();
}
