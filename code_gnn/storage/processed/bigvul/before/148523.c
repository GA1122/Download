void WebContentsImpl::ReloadFocusedFrame(bool bypass_cache) {
  RenderFrameHost* focused_frame = GetFocusedFrame();
  if (!focused_frame)
    return;

  focused_frame->Send(new FrameMsg_Reload(
      focused_frame->GetRoutingID(), bypass_cache));
}
