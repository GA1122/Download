  void ForceTimeout() {
    if (IsRunning()) {
      user_task().Run();
      Stop();
    }
  }
