void DevToolsUIBindings::AgentHostClosed(
    content::DevToolsAgentHost* agent_host,
    bool replaced_with_another_client) {
  DCHECK(agent_host == agent_host_.get());
  agent_host_ = NULL;
  delegate_->InspectedContentsClosing();
}
