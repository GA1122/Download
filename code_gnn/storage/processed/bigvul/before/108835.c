bool RenderViewImpl::CanComposeInline() {
  return pepper_delegate_.IsPluginFocused() ?
      pepper_delegate_.CanComposeInline() : true;
}
