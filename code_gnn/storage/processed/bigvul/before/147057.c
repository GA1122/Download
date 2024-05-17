WebURLRequest WebLocalFrameImpl::RequestForReload(
    WebFrameLoadType load_type,
    const WebURL& override_url) const {
  DCHECK(GetFrame());
  ResourceRequest request = GetFrame()->Loader().ResourceRequestForReload(
      static_cast<FrameLoadType>(load_type), override_url);
  return WrappedResourceRequest(request);
}
