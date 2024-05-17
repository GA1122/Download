void PrintWebViewHelper::PrintPreviewContext::InitWithFrame(
    WebKit::WebFrame* web_frame) {
  DCHECK(web_frame);
  state_ = INITIALIZED;
  frame_ = web_frame;
  node_.reset();
}
