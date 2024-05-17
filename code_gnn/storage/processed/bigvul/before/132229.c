void RenderFrameImpl::NavigateToSwappedOutURL() {
  is_swapped_out_ = true;
  GURL swappedOutURL(kSwappedOutURL);
  WebURLRequest request(swappedOutURL);
  frame_->loadRequest(request);
}
