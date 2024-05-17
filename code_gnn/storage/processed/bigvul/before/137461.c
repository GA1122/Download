 void MessageLoop::SetThreadTaskRunnerHandle() {
   DCHECK_EQ(this, current());
  thread_task_runner_handle_.reset();
  thread_task_runner_handle_.reset(new ThreadTaskRunnerHandle(task_runner_));
}
