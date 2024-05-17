WebLocalFrameImpl* WebLocalFrameImpl::Create(
    WebTreeScopeType scope,
    WebLocalFrameClient* client,
    blink::InterfaceRegistry* interface_registry,
    WebFrame* opener) {
  WebLocalFrameImpl* frame =
      new WebLocalFrameImpl(scope, client, interface_registry);
  frame->SetOpener(opener);
  return frame;
}
