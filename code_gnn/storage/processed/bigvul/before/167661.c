void RenderFrameHostCreatedObserver::Wait() {
  message_loop_runner_->Run();
}
