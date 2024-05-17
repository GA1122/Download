void RenderFrameImpl::OnAdjustSelectionByCharacterOffset(
    int start_adjust,
    int end_adjust,
    bool show_selection_menu) {
  WebRange range = frame_->GetInputMethodController()->GetSelectionOffsets();
  if (range.IsNull())
    return;

  if (start_adjust - end_adjust > range.length() ||
      range.StartOffset() + start_adjust < 0)
    return;

  AutoResetMember<bool> handling_select_range(
      this, &RenderFrameImpl::handling_select_range_, true);

  frame_->SelectRange(WebRange(range.StartOffset() + start_adjust,
                               range.length() + end_adjust - start_adjust),
                      WebLocalFrame::kPreserveHandleVisibility,
                      show_selection_menu ? SelectionMenuBehavior::kShow
                                          : SelectionMenuBehavior::kHide);
}
