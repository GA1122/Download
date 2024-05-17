void DevToolsUIBindings::Detach() {
  if (agent_host_.get())
    agent_host_->DetachClient(this);
  agent_host_ = NULL;
}
