NaClIPCAdapter::NaClIPCAdapter(const IPC::ChannelHandle& handle,
                               base::TaskRunner* runner)
    : lock_(),
      cond_var_(&lock_),
      task_runner_(runner),
      locked_data_() {
  io_thread_data_.channel_.reset(
      new IPC::Channel(handle, IPC::Channel::MODE_SERVER, this));
  task_runner_->PostTask(FROM_HERE,
      base::Bind(&NaClIPCAdapter::ConnectChannelOnIOThread, this));
}
