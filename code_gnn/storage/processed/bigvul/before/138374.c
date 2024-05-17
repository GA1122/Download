  void RemoveConnectionFilter(int filter_id) {
    io_task_runner_->PostTask(
        FROM_HERE,
        base::Bind(&IOThreadContext::RemoveConnectionFilterOnIOThread, this,
                   filter_id));
  }
