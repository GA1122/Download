GURL RenderFrameImpl::GetLoadingUrl() const {
  WebDocumentLoader* document_loader = frame_->GetDocumentLoader();

  GURL overriden_url;
  if (MaybeGetOverriddenURL(document_loader, &overriden_url))
    return overriden_url;

  const WebURLRequest& request = document_loader->GetRequest();
  return request.Url();
}
