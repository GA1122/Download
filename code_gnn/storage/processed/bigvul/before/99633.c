void VaapiVideoDecodeAccelerator::Flush() {
  DCHECK_EQ(message_loop_, base::MessageLoop::current());
  DVLOG(1) << "Got flush request";

  base::AutoLock auto_lock(lock_);
  state_ = kFlushing;
  decoder_thread_task_runner_->PostTask(
      FROM_HERE, base::Bind(&VaapiVideoDecodeAccelerator::FlushTask,
                            base::Unretained(this)));

  input_ready_.Signal();
  surfaces_available_.Signal();
}
