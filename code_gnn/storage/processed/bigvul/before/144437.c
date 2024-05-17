void WebContentsImpl::AdjustSelectionByCharacterOffset(int start_adjust,
                                                       int end_adjust) {
  RenderFrameHost* focused_frame = GetFocusedFrame();
  if (!focused_frame)
    return;

  focused_frame->Send(new InputMsg_AdjustSelectionByCharacterOffset(
      focused_frame->GetRoutingID(), start_adjust, end_adjust));
}
