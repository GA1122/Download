void RenderViewImpl::didChangeSelection(bool is_empty_selection) {
  if (!handling_input_event_ && !handling_select_range_)
      return;
  handling_select_range_ = false;

  if (is_empty_selection)
    selection_text_.clear();

  SyncSelectionIfRequired();
}
