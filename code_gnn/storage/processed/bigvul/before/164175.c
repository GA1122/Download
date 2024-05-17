  void FlushAllTasks() {
    FlushTasks(storage()->db_task_runner_.get());
    FlushTasks(background_thread->task_runner().get());
    FlushTasks(storage()->db_task_runner_.get());
  }
