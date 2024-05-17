void BlinkTestRunner::DidFailProvisionalLoad(WebLocalFrame* frame,
                                             const WebURLError& error) {
  focus_on_next_commit_ = false;
}
