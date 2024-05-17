void DevToolsUIBindings::Reattach(const DispatchCallback& callback) {
  if (agent_host_.get()) {
    agent_host_->DetachClient(this);
    agent_host_->AttachClient(this);
  }
  callback.Run(nullptr);
}
