  TaskRunner* file_task_runner() const {
    return file_thread_.message_loop_proxy().get();
  }
