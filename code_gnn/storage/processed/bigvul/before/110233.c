void NaClIPCAdapter::CloseChannel() {
  {
    base::AutoLock lock(lock_);
    locked_data_.channel_closed_ = true;
  }
  cond_var_.Signal();

  task_runner_->PostTask(FROM_HERE,
      base::Bind(&NaClIPCAdapter::CloseChannelOnIOThread, this));
}
