blink::WebRemoteFrame* RenderFrameImpl::AdoptPortal(
    const base::UnguessableToken& portal_token) {
  int proxy_routing_id = MSG_ROUTING_NONE;
  GetFrameHost()->AdoptPortal(portal_token, &proxy_routing_id);
  RenderFrameProxy* proxy =
      RenderFrameProxy::CreateProxyForPortal(this, proxy_routing_id);
  return proxy->web_frame();
}
