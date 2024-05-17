void RenderFrameImpl::DidChangeSelection(bool is_empty_selection) {
  if (!GetRenderWidget()->input_handler().handling_input_event() &&
      !handling_select_range_)
    return;

  if (is_empty_selection)
    selection_text_.clear();

  GetRenderWidget()->UpdateTextInputState();
  SyncSelectionIfRequired();
}
