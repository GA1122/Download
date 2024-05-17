void DevToolsAgent::sendMessageToInspectorFrontend(
    const WebKit::WebString& message) {
  Send(new DevToolsHostMsg_ForwardToClient(
      routing_id(),
      DevToolsClientMsg_DispatchOnInspectorFrontend(MSG_ROUTING_NONE,
                                                    message.utf8())));
}
