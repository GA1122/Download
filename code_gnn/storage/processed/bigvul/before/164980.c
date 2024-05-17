cc::Layer* HTMLCanvasElement::ContentsCcLayer() const {
  if (surface_layer_bridge_)
    return surface_layer_bridge_->GetCcLayer();
  if (context_ && context_->IsComposited())
    return context_->CcLayer();
  return nullptr;
}
