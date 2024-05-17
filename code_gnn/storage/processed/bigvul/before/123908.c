void RenderViewImpl::OnSetBackground(const SkBitmap& background) {
  if (webview())
    webview()->setIsTransparent(!background.empty());
  if (compositor_)
    compositor_->setHasTransparentBackground(!background.empty());

  SetBackground(background);
}
