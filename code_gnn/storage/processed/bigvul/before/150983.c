void DevToolsUIBindings::DocumentAvailableInMainFrame() {
  if (!reloading_)
    return;
  reloading_ = false;
  if (agent_host_.get()) {
    agent_host_->DetachClient(this);
    agent_host_->AttachClient(this);
  }
}
