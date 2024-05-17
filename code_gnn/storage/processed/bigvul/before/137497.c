  void BindToCurrentThread() {
    thread_task_runner_handle_ =
        MakeUnique<ThreadTaskRunnerHandle>(simple_task_runner_);
     run_loop_client_ = RunLoop::RegisterDelegateForCurrentThread(this);
   }
