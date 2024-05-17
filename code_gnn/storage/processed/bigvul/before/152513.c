void RenderFrameImpl::ScrollRectToVisibleInParentFrame(
    const blink::WebRect& rect_to_scroll,
    const blink::WebScrollIntoViewParams& params) {
  DCHECK(IsLocalRoot());
  Send(new FrameHostMsg_ScrollRectToVisibleInParentFrame(
      routing_id_, rect_to_scroll, params));
}
