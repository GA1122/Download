bool RenderView::CanComposeInline() {
  if (pepper_delegate_.IsPluginFocused()) {
    return false;
  }
  return true;
}
