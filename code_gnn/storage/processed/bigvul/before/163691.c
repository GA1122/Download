void WebContentsImpl::AdjustSelectionByCharacterOffset(int start_adjust,
                                                       int end_adjust) {
  RenderFrameHostImpl* focused_frame = GetFocusedFrame();
  if (!focused_frame)
    return;

  focused_frame->GetFrameInputHandler()->AdjustSelectionByCharacterOffset(
      start_adjust, end_adjust);
}
