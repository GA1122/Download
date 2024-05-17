void RenderProcessHostWatcher::RenderProcessHostDestroyed(
    RenderProcessHost* host) {
  render_process_host_ = nullptr;
  if (type_ == WATCH_FOR_HOST_DESTRUCTION)
    message_loop_runner_->Quit();
}
