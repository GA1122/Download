void WebContentsImpl::AdjustSelectionByCharacterOffset(
    int start_adjust,
    int end_adjust,
    bool show_selection_menu) {
  RenderFrameHostImpl* focused_frame = GetFocusedFrame();
  if (!focused_frame)
    return;

  using blink::mojom::SelectionMenuBehavior;
  focused_frame->GetFrameInputHandler()->AdjustSelectionByCharacterOffset(
      start_adjust, end_adjust,
      show_selection_menu ? SelectionMenuBehavior::kShow
                          : SelectionMenuBehavior::kHide);
}
