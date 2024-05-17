  void InvokeOnIO(const base::Closure& task) {
    fake_io_thread_work_ = task;
    fake_io_thread_completion_.Signal();
    content::RunMessageLoop();
  }
