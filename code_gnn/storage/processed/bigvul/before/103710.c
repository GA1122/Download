void DevToolsAgent::sendDebuggerOutput(const WebKit::WebString& data) {
  Send(new DevToolsHostMsg_ForwardToClient(
      routing_id(),
      DevToolsClientMsg_DebuggerOutput(MSG_ROUTING_NONE, data.utf8())));
}
