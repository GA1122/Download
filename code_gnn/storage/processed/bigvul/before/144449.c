void WebContentsImpl::CopyToFindPboard() {
#if defined(OS_MACOSX)
  RenderFrameHost* focused_frame = GetFocusedFrame();
  if (!focused_frame)
    return;

  focused_frame->Send(
      new InputMsg_CopyToFindPboard(focused_frame->GetRoutingID()));
  RecordAction(base::UserMetricsAction("CopyToFindPboard"));
#endif
}
