void RenderViewImpl::DidFocus(blink::WebLocalFrame* calling_frame) {
  if (WebUserGestureIndicator::IsProcessingUserGesture(calling_frame) &&
      !RenderThreadImpl::current()->layout_test_mode()) {
    Send(new ViewHostMsg_Focus(GetRoutingID()));

    RenderFrameImpl* calling_render_frame =
        RenderFrameImpl::FromWebFrame(calling_frame);
    if (calling_render_frame)
      calling_render_frame->FrameDidCallFocus();
  }
}
