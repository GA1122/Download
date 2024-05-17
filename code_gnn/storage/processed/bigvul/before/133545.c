int WebContentsImpl::CreateOpenerRenderViews(SiteInstance* instance) {
  int opener_route_id = MSG_ROUTING_NONE;

  if (opener_)
    opener_route_id = opener_->CreateOpenerRenderViews(instance);

  if (GetRenderManager()->current_host()->GetSiteInstance() == instance)
    return GetRenderManager()->current_host()->GetRoutingID();

  if (GetRenderManager()->pending_render_view_host() &&
      GetRenderManager()->pending_render_view_host()->GetSiteInstance() ==
          instance)
    return GetRenderManager()->pending_render_view_host()->GetRoutingID();

  RenderViewHostImpl* rvh = GetRenderManager()->GetSwappedOutRenderViewHost(
      instance);
  if (rvh)
    return rvh->GetRoutingID();

  return GetRenderManager()->CreateRenderFrame(instance, opener_route_id,
                                               true, true);
}
