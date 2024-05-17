  bool ProcessTask() {
    OnceClosure task;
    {
      AutoLock auto_lock(tasks_lock_);
      if (pending_tasks_.empty())
        return false;
      task = std::move(pending_tasks_.front());
      pending_tasks_.pop();
    }
    std::move(task).Run();
    return true;
  }
