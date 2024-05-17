void RenderFrameImpl::DidChangeIcon(blink::WebIconURL::Type icon_type) {
  SendUpdateFaviconURL(icon_type);
}
