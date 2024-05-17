  void WaitUntilScreenshotIsReady() {
    if (!encoding_screenshot_in_progress_)
      return;
    message_loop_runner_ = new content::MessageLoopRunner;
    message_loop_runner_->Run();
  }
