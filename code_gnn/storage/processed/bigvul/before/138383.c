  void Start(const base::Closure& stop_callback) {
    DCHECK(!started_);

    started_ = true;
    callback_task_runner_ = base::ThreadTaskRunnerHandle::Get();
    stop_callback_ = stop_callback;
    io_task_runner_->PostTask(
        FROM_HERE, base::Bind(&IOThreadContext::StartOnIOThread, this));
  }
