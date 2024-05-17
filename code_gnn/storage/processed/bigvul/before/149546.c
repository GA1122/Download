  explicit ConnectionListener(ConnectionTracker* tracker)
      : task_runner_(base::ThreadTaskRunnerHandle::Get()), tracker_(tracker) {}
