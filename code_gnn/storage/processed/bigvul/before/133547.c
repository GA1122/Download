int WebContentsImpl::CreateSwappedOutRenderView(
    SiteInstance* instance) {
  return GetRenderManager()->CreateRenderFrame(instance, MSG_ROUTING_NONE,
                                               true, true);
}
