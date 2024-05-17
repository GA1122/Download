void WebContentsLoadedOrDestroyedWatcher::Wait() {
  message_loop_runner_->Run();
}
