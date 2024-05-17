void RenderViewImpl::NavigateToSwappedOutURL(WebKit::WebFrame* frame) {
  GURL swappedOutURL(content::kSwappedOutURL);
  WebURLRequest request(swappedOutURL);
  frame->loadRequest(request);
}
