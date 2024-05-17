  void LoopSyncShare() {
    bool should_loop = false;
    int loop_iterations = 0;
    do {
      ASSERT_LT(++loop_iterations, 100) << "infinite loop detected. please fix";
      should_loop = SyncShareAsDelegate();
    } while (should_loop);
  }
