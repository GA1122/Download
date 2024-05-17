void RenderViewImpl::OnExtendSelectionAndDelete(int before, int after) {
  if (!webview())
    return;
  webview()->extendSelectionAndDelete(before, after);
}
