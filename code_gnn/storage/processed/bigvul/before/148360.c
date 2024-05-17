void WebContentsImpl::DispatchBeforeUnload() {
  bool for_cross_site_transition = false;
  GetMainFrame()->DispatchBeforeUnload(for_cross_site_transition, false);
}
