void RenderFrameHostImpl::AdoptPortal(
    const base::UnguessableToken& portal_token,
    AdoptPortalCallback callback) {
  Portal* portal = Portal::FromToken(portal_token);
  if (!portal) {
    mojo::ReportBadMessage("Unknown portal_token when adopting portal.");
    return;
  }
  if (portal->owner_render_frame_host() != this) {
    mojo::ReportBadMessage("AdoptPortal called from wrong frame.");
    return;
  }
  RenderFrameProxyHost* proxy_host = portal->CreateProxyAndAttachPortal();
  std::move(callback).Run(proxy_host->GetRoutingID());
}
