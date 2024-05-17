bool RenderFrameImpl::AllowContentInitiatedDataUrlNavigations(
    const blink::WebURL& url) {
  return url.GetString() == kUnreachableWebDataURL;
}
