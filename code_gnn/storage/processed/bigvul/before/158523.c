WebLocalFrameImpl* WebLocalFrameImpl::CreateProvisional(
    WebLocalFrameClient* client,
    blink::InterfaceRegistry* interface_registry,
    WebRemoteFrame* old_web_frame,
    WebSandboxFlags flags,
    ParsedFeaturePolicy container_policy) {
  DCHECK(client);
  WebLocalFrameImpl* web_frame =
      new WebLocalFrameImpl(old_web_frame, client, interface_registry);
  Frame* old_frame = ToWebRemoteFrameImpl(old_web_frame)->GetFrame();
  web_frame->SetParent(old_web_frame->Parent());
  web_frame->SetOpener(old_web_frame->Opener());
  web_frame->InitializeCoreFrame(*old_frame->GetPage(),
                                 DummyFrameOwner::Create(),
                                 old_frame->Tree().GetName());

  LocalFrame* new_frame = web_frame->GetFrame();
  new_frame->SetOwner(old_frame->Owner());
  if (new_frame->Owner() && new_frame->Owner()->IsRemote()) {
    ToRemoteFrameOwner(new_frame->Owner())
        ->SetSandboxFlags(static_cast<SandboxFlags>(flags));
    ToRemoteFrameOwner(new_frame->Owner())
        ->SetContainerPolicy(container_policy);
  } else if (!new_frame->Owner()) {
    new_frame->Loader().ForceSandboxFlags(static_cast<SandboxFlags>(flags));
  }

  return web_frame;
}
