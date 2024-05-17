void RenderViewImpl::OnSetBackground(const SkBitmap& background) {
  if (webview())
    webview()->setIsTransparent(!background.empty());
  if (web_layer_tree_view_)
    web_layer_tree_view_->setHasTransparentBackground(!background.empty());

  SetBackground(background);
}
