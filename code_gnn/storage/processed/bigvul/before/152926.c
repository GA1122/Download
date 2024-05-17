void DevToolsClient::sendMessageToEmbedder(const WebString& message) {
  Send(new DevToolsHostMsg_DispatchOnEmbedder(routing_id(),
                                              message.utf8()));
}
