void RemoteFrame::SetWebLayer(WebLayer* web_layer) {
  if (web_layer_)
    GraphicsLayer::UnregisterContentsLayer(web_layer_);
  web_layer_ = web_layer;
  if (web_layer_)
    GraphicsLayer::RegisterContentsLayer(web_layer_);

  DCHECK(Owner());
  ToHTMLFrameOwnerElement(Owner())->SetNeedsCompositingUpdate();
}
