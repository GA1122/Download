WebLocalFrameImpl::WebLocalFrameImpl(
    WebRemoteFrame* old_web_frame,
    WebFrameClient* client,
    blink::InterfaceRegistry* interface_registry)
    : WebLocalFrameImpl(old_web_frame->InShadowTree()
                            ? WebTreeScopeType::kShadow
                            : WebTreeScopeType::kDocument,
                        client,
                        interface_registry) {}
