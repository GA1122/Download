  void TearDownTest() {
    DCHECK(io_runner->BelongsToCurrentThread());
    scoped_refptr<base::SequencedTaskRunner> db_runner =
        storage()->db_task_runner_;
    storage()->CancelDelegateCallbacks(delegate());
    group_ = nullptr;
    cache_ = nullptr;
    cache2_ = nullptr;
    mock_quota_manager_proxy_ = nullptr;
    delegate_.reset();
    service_.reset();
    FlushTasks(db_runner.get());
    FlushTasks(background_thread->task_runner().get());
    FlushTasks(db_runner.get());
  }
