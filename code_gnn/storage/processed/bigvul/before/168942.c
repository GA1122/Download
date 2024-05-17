bool DevToolsAgentHostImpl::ShouldForceCreation() {
  return !!s_force_creation_count_;
}
