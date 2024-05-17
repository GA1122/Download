void HeadlessWebContentsImpl::ForceAttachClient(
    HeadlessDevToolsClient* client) {
  HeadlessDevToolsClientImpl::From(client)->ForceAttachToHost(
      agent_host_.get());
}
