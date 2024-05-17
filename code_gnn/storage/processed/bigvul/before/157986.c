void RenderViewImpl::SetFocusAndActivateForTesting(bool enable) {
  if (webview()->MainFrame()->IsWebRemoteFrame())
    return;

  if (enable == has_focus())
    return;

  if (enable) {
    SetActiveForWidget(true);
    OnSetFocus(true);
  } else {
    OnSetFocus(false);
    SetActiveForWidget(false);
  }
}
