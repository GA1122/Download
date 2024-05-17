void WebContentsImpl::PasteAndMatchStyle() {
  RenderFrameHost* focused_frame = GetFocusedFrame();
  if (!focused_frame)
    return;

  focused_frame->GetFrameInputHandler()->PasteAndMatchStyle();
  RecordAction(base::UserMetricsAction("PasteAndMatchStyle"));
}
