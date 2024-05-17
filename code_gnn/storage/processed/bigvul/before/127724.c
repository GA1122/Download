  void PushTask(const base::Closure& task) {
    task_queue_.push(task);
  }
