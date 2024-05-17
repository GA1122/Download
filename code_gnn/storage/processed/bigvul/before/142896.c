void HTMLMediaElement::SetCcLayer(cc::Layer* cc_layer) {
  if (cc_layer == cc_layer_)
    return;

  if (!cc_layer_ || !cc_layer)
    SetNeedsCompositingUpdate();

  if (cc_layer_)
    GraphicsLayer::UnregisterContentsLayer(cc_layer_);
  cc_layer_ = cc_layer;
  if (cc_layer_)
    GraphicsLayer::RegisterContentsLayer(cc_layer_);
}
