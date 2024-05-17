  void RunCurrentTasks() {
    size_t posted_tasks_size = runner_->GetPostedTasks().size();
    for (size_t i = 0; i < posted_tasks_size; ++i) {
      runner_->RunNextTask();
    }
  }
