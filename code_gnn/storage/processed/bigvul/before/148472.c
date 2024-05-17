void WebContentsImpl::OnDidRunContentWithCertificateErrors(
    RenderFrameHostImpl* source,
    const GURL& url) {
  NavigationEntry* entry = controller_.GetVisibleEntry();
  if (!entry)
    return;

  controller_.ssl_manager()->DidRunContentWithCertErrors(
      entry->GetURL().GetOrigin());
}
