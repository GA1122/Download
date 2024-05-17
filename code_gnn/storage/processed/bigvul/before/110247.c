NaClIPCAdapter::NaClIPCAdapter(scoped_ptr<IPC::Channel> channel,
                               base::TaskRunner* runner)
    : lock_(),
      cond_var_(&lock_),
      task_runner_(runner),
      locked_data_() {
  io_thread_data_.channel_ = channel.Pass();
}
