Canvas2DLayerBridge* HTMLCanvasElement::GetOrCreateCanvas2DLayerBridge() {
  DCHECK(Is2d());
  if (!canvas2d_bridge_ && !did_fail_to_create_resource_provider_) {
    SetCanvas2DLayerBridgeInternal(nullptr);
    if (did_fail_to_create_resource_provider_ && !Size().IsEmpty())
      context_->LoseContext(CanvasRenderingContext::kSyntheticLostContext);
  }
  return canvas2d_bridge_.get();
}
