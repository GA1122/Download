bool ShouldRespondToRequest(blink::WebLocalFrame** frame_ptr,
                            RenderFrame** render_frame_ptr) {
  blink::WebLocalFrame* frame = blink::WebLocalFrame::FrameForCurrentContext();
  if (!frame || !frame->View())
    return false;

  GURL frame_url = frame->GetDocument().Url();

  RenderFrame* render_frame = RenderFrame::FromWebFrame(frame);
  if (!render_frame)
    return false;

  bool webui_enabled =
      (render_frame->GetEnabledBindings() & BINDINGS_POLICY_WEB_UI) &&
      (frame_url.SchemeIs(kChromeUIScheme) ||
       frame_url.SchemeIs(url::kDataScheme));

  if (!webui_enabled)
    return false;

  *frame_ptr = frame;
  *render_frame_ptr = render_frame;
  return true;
}
