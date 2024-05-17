void BlinkTestRunner::DidCommitProvisionalLoad(WebLocalFrame* frame,
                                               bool is_new_navigation) {
  if (!focus_on_next_commit_)
    return;
  focus_on_next_commit_ = false;
  render_view()->GetWebView()->setFocusedFrame(frame);
}
