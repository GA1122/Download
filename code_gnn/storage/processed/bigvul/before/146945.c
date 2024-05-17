WebLocalFrame* WebLocalFrame::CreateProvisional(
    WebFrameClient* client,
    InterfaceRegistry* interface_registry,
    WebRemoteFrame* old_web_frame,
    WebSandboxFlags flags,
    WebParsedFeaturePolicy container_policy) {
  return WebLocalFrameImpl::CreateProvisional(
      client, interface_registry, old_web_frame, flags, container_policy);
}
