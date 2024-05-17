void RenderFrameImpl::OnCollapseSelection() {
  const WebRange& range =
      frame_->GetInputMethodController()->GetSelectionOffsets();
  if (range.IsNull())
    return;

  AutoResetMember<bool> handling_select_range(
      this, &RenderFrameImpl::handling_select_range_, true);
  frame_->SelectRange(WebRange(range.EndOffset(), 0),
                      WebLocalFrame::kHideSelectionHandle,
                      SelectionMenuBehavior::kHide);
}
