  static void TearDownTestCase() {
    io_runner.reset();
    background_thread.reset();
    scoped_task_environment.reset();
  }
