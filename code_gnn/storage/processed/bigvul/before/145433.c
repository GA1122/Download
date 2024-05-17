  void RunUntilCallbacksFired() {
    while (callback_counter_ != 0) {
      ASSERT_GT(task_runner_->GetPostedTasks().size(), 0u);
      task_runner_->RunNextTask();
    }
  }
