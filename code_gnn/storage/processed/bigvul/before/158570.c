void WebLocalFrameImpl::SaveImageAt(const WebPoint& pos_in_viewport) {
  Node* node = HitTestResultForVisualViewportPos(pos_in_viewport)
                   .InnerNodeOrImageMapImage();
  if (!node || !(IsHTMLCanvasElement(*node) || IsHTMLImageElement(*node)))
    return;

  String url = ToElement(*node).ImageSourceURL();
  if (!KURL(NullURL(), url).ProtocolIsData())
    return;

  client_->SaveImageFromDataURL(url);
}
