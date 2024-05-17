void WebContentsImpl::EnableWebContentsOnlyAccessibilityMode() {
  if (!GetAccessibilityMode().is_mode_off()) {
    for (RenderFrameHost* rfh : GetAllFrames())
      ResetAccessibility(rfh);
  } else {
    AddAccessibilityMode(ui::kAXModeWebContentsOnly);
  }
}
