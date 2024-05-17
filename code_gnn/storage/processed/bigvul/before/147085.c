void WebLocalFrameImpl::SetIsolatedWorldContentSecurityPolicy(
    int world_id,
    const WebString& policy) {
  DCHECK(GetFrame());
  DOMWrapperWorld::SetIsolatedWorldContentSecurityPolicy(world_id, policy);
}
