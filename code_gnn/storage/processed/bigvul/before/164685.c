void RenderFrameHostImpl::CreatePortal(blink::mojom::PortalRequest request,
                                       CreatePortalCallback callback) {
  if (frame_tree_node()->parent()) {
    mojo::ReportBadMessage(
        "RFHI::CreatePortal called in a nested browsing context");
    return;
  }
  Portal* portal = Portal::Create(this, std::move(request));
  RenderFrameProxyHost* proxy_host = portal->CreateProxyAndAttachPortal();
  std::move(callback).Run(proxy_host->GetRoutingID(), portal->portal_token());
}
