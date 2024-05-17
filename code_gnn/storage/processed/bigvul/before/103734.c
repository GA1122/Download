void DevToolsClient::sendDebuggerCommandToAgent(const WebString& command) {
  SendToAgent(DevToolsAgentMsg_DebuggerCommand(MSG_ROUTING_NONE,
                                               command.utf8()));
}
