void DevToolsClient::activateWindow() {
  Send(new DevToolsHostMsg_ActivateWindow(routing_id()));
}
