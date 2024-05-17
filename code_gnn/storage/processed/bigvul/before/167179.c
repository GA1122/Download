void HTMLMediaElement::SetWebLayer(WebLayer* web_layer) {
  if (web_layer == web_layer_)
    return;

  if (!web_layer_ || !web_layer)
    SetNeedsCompositingUpdate();

  if (web_layer_)
    GraphicsLayer::UnregisterContentsLayer(web_layer_);
  web_layer_ = web_layer;
  if (web_layer_)
    GraphicsLayer::RegisterContentsLayer(web_layer_);
}
