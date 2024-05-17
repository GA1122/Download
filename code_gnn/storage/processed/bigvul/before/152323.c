RenderFrame* RenderFrame::FromWebFrame(blink::WebLocalFrame* web_frame) {
  return RenderFrameImpl::FromWebFrame(web_frame);
}
