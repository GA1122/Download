bool RenderFrameImpl::willCheckAndDispatchMessageEvent(
    blink::WebLocalFrame* source_frame,
    blink::WebFrame* target_frame,
    blink::WebSecurityOrigin target_origin,
    blink::WebDOMMessageEvent event) {
  DCHECK(!frame_ || frame_ == target_frame);

  if (!is_swapped_out_)
    return false;

  if (!render_frame_proxy_)
    return false;

  render_frame_proxy_->postMessageEvent(
      source_frame, render_frame_proxy_->web_frame(), target_origin, event);
  return true;
}
