DevToolsAgent::~DevToolsAgent() {
  agent_for_routing_id_.erase(routing_id());
}
