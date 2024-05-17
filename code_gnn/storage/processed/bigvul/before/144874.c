  void StopVirtualTimeAndExitRunLoop() {
    WebView().Scheduler()->SetVirtualTimePolicy(
        PageScheduler::VirtualTimePolicy::kPause);
    testing::ExitRunLoop();
  }
