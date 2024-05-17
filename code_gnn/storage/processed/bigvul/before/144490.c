void WebContentsImpl::DispatchBeforeUnload(bool for_cross_site_transition) {
  GetMainFrame()->DispatchBeforeUnload(for_cross_site_transition);
}
