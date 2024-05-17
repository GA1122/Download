WebLocalFrameImpl* WebLocalFrameImpl::Create(
    WebTreeScopeType scope,
    WebFrameClient* client,
    blink::InterfaceRegistry* interface_registry,
    WebFrame* opener) {
  WebLocalFrameImpl* frame =
      new WebLocalFrameImpl(scope, client, interface_registry);
  frame->SetOpener(opener);
  return frame;
}
