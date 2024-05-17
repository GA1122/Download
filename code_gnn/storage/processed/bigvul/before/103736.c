void DevToolsClient::sendMessageToBackend(const WebString& message)  {
  SendToAgent(DevToolsAgentMsg_DispatchOnInspectorBackend(MSG_ROUTING_NONE,
                                                          message.utf8()));
}
