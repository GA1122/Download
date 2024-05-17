void WebLocalFrameImpl::SaveImageAt(const WebPoint& pos_in_viewport) {
  Node* node = HitTestResultForVisualViewportPos(pos_in_viewport)
                   .InnerNodeOrImageMapImage();
  if (!node || !(isHTMLCanvasElement(*node) || isHTMLImageElement(*node)))
    return;

  String url = ToElement(*node).ImageSourceURL();
  if (!KURL(NullURL(), url).ProtocolIsData())
    return;

  client_->SaveImageFromDataURL(url);
}
