void VaapiVideoDecodeAccelerator::FinishFlush() {
  DCHECK_EQ(message_loop_, base::MessageLoop::current());

  finish_flush_pending_ = false;

  base::AutoLock auto_lock(lock_);
  if (state_ != kFlushing) {
    DCHECK_EQ(state_, kDestroying);
    return;   
  }

  if (!pending_output_cbs_.empty()) {
    finish_flush_pending_ = true;
    return;
  }

  state_ = kIdle;

  message_loop_->PostTask(FROM_HERE, base::Bind(
      &Client::NotifyFlushDone, client_));

  DVLOG(1) << "Flush finished";
}
