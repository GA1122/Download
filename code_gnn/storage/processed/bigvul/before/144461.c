int WebContentsImpl::CreateSwappedOutRenderView(
    SiteInstance* instance) {
  int render_view_routing_id = MSG_ROUTING_NONE;
  if (SiteIsolationPolicy::IsSwappedOutStateForbidden()) {
    GetRenderManager()->CreateRenderFrameProxy(instance);
  } else {
    GetRenderManager()->CreateRenderFrame(
        instance, CREATE_RF_SWAPPED_OUT | CREATE_RF_HIDDEN,
        &render_view_routing_id);
  }
  return render_view_routing_id;
}
