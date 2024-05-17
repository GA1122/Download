RenderFrameImpl::CreatePortal(mojo::ScopedInterfaceEndpointHandle pipe) {
  int proxy_routing_id = MSG_ROUTING_NONE;
  base::UnguessableToken portal_token;
  GetFrameHost()->CreatePortal(
      blink::mojom::PortalAssociatedRequest(std::move(pipe)), &proxy_routing_id,
      &portal_token);
  RenderFrameProxy* proxy =
      RenderFrameProxy::CreateProxyForPortal(this, proxy_routing_id);
  return std::make_pair(proxy->web_frame(), portal_token);
}
