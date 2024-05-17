NaClIPCAdapter::~NaClIPCAdapter() {
  task_runner_->PostTask(FROM_HERE,
      base::Bind(&DeleteChannel, io_thread_data_.channel_.release()));
}
