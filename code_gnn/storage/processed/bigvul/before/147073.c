void WebLocalFrameImpl::SetCaretVisible(bool visible) {
  GetFrame()->Selection().SetCaretVisible(visible);
}
