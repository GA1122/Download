void RenderViewImpl::OnSetEditableSelectionOffsets(int start, int end) {
  base::AutoReset<bool> handling_select_range(&handling_select_range_, true);
  DCHECK(!handling_ime_event_);
  handling_ime_event_ = true;
  webview()->setEditableSelectionOffsets(start, end);
  handling_ime_event_ = false;
  UpdateTextInputState(DO_NOT_SHOW_IME);
}
