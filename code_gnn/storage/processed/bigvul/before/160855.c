WebLocalFrame* CreateChildCounterFrameClient::CreateChildFrame(
    WebLocalFrame* parent,
    WebTreeScopeType scope,
    const WebString& name,
    const WebString& fallback_name,
    WebSandboxFlags sandbox_flags,
    const ParsedFeaturePolicy& container_policy,
    const WebFrameOwnerProperties& frame_owner_properties) {
  ++count_;
  return TestWebFrameClient::CreateChildFrame(
      parent, scope, name, fallback_name, sandbox_flags, container_policy,
      frame_owner_properties);
}
