void MessageLoop::ClearTaskRunnerForTesting() {
  DCHECK_EQ(this, current());
  DCHECK(!unbound_task_runner_);
  task_runner_ = nullptr;
  thread_task_runner_handle_.reset();
}
