void DevToolsClient::SendToAgent(const IPC::Message& tools_agent_message) {
  Send(new DevToolsHostMsg_ForwardToAgent(routing_id(), tools_agent_message));
}
