void WebContentsImpl::CopyToFindPboard() {
#if defined(OS_MACOSX)
  RenderFrameHost* focused_frame = GetFocusedFrame();
  if (!focused_frame)
    return;

  focused_frame->GetFrameInputHandler()->CopyToFindPboard();
  RecordAction(base::UserMetricsAction("CopyToFindPboard"));
#endif
}
