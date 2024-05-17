WebLocalFrameImpl* WebLocalFrameImpl::CreateLocalChild(
    WebTreeScopeType scope,
    WebLocalFrameClient* client,
    blink::InterfaceRegistry* interface_registry) {
  WebLocalFrameImpl* frame =
      new WebLocalFrameImpl(scope, client, interface_registry);
  AppendChild(frame);
  return frame;
}
