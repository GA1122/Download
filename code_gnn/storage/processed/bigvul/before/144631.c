void WebContentsImpl::ReloadFocusedFrame(bool ignore_cache) {
  RenderFrameHost* focused_frame = GetFocusedFrame();
  if (!focused_frame)
    return;

  focused_frame->Send(new FrameMsg_Reload(
      focused_frame->GetRoutingID(), ignore_cache));
}
