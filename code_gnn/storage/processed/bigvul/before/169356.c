void RenderProcessHostWatcher::Wait() {
  message_loop_runner_->Run();
}
