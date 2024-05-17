SiteInstance* WebContentsImpl::GetPendingSiteInstance() const {
  RenderViewHost* dest_rvh = GetRenderManager()->pending_render_view_host() ?
      GetRenderManager()->pending_render_view_host() :
      GetRenderManager()->current_host();
  return dest_rvh->GetSiteInstance();
}
