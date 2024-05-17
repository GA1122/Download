void DevToolsClient::sendFrontendLoaded() {
  SendToAgent(DevToolsAgentMsg_FrontendLoaded(MSG_ROUTING_NONE));
}
