bool HeadlessWebContentsImpl::AttachClient(HeadlessDevToolsClient* client) {
  return HeadlessDevToolsClientImpl::From(client)->AttachToHost(
      agent_host_.get());
}
