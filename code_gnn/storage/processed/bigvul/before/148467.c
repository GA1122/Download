void WebContentsImpl::OnDidDisplayContentWithCertificateErrors(
    RenderFrameHostImpl* source,
    const GURL& url) {
  controller_.ssl_manager()->DidDisplayContentWithCertErrors();
}
