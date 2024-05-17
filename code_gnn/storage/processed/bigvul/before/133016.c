void RenderWidgetHostViewAura::SelectionBoundsChanged(
    const ViewHostMsg_SelectionBounds_Params& params) {
  if (selection_anchor_rect_ == params.anchor_rect &&
      selection_focus_rect_ == params.focus_rect)
    return;

  selection_anchor_rect_ = params.anchor_rect;
  selection_focus_rect_ = params.focus_rect;

  if (GetInputMethod())
    GetInputMethod()->OnCaretBoundsChanged(this);

  if (touch_editing_client_) {
    touch_editing_client_->OnSelectionOrCursorChanged(selection_anchor_rect_,
        selection_focus_rect_);
  }
}
