void TaskQueueManager::MaybePostDoWorkOnMainRunner() {
  bool on_main_thread = main_task_runner_->BelongsToCurrentThread();
  if (on_main_thread) {
    if (pending_dowork_count_ > 0) {
      return;
    }
    pending_dowork_count_++;
    main_task_runner_->PostTask(FROM_HERE, do_work_from_main_thread_closure_);
  } else {
    main_task_runner_->PostTask(FROM_HERE, do_work_from_other_thread_closure_);
  }
}
