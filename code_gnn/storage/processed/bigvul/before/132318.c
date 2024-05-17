void RenderFrameImpl::didReceiveServerRedirectForProvisionalLoad(
    blink::WebLocalFrame* frame) {
  DCHECK(!frame_ || frame_ == frame);
  render_view_->history_controller()->RemoveChildrenForRedirect(this);
}
