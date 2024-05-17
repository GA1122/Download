void RenderViewImpl::didChangeIcon(WebFrame* frame, WebIconURL::Type type) {
  favicon_helper_->DidChangeIcon(frame, type);
}
