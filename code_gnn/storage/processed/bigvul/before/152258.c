void RenderFrameImpl::DidChangeSelection(bool is_empty_selection) {
  if (!GetLocalRootRenderWidget()->input_handler().handling_input_event() &&
      !handling_select_range_)
    return;

  if (is_empty_selection)
    selection_text_.clear();

  GetLocalRootRenderWidget()->UpdateTextInputState();
  SyncSelectionIfRequired();
}
