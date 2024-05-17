WebKit::WebFrame* PrintWebViewHelper::PrintPreviewContext::frame() {
  CHECK(state_ != UNINITIALIZED);
  return frame_;
}
