  void ShutDown() {
    if (!started_)
      return;

    bool posted = io_task_runner_->PostTask(
        FROM_HERE, base::Bind(&IOThreadContext::ShutDownOnIOThread, this));
    DCHECK(posted);
  }
