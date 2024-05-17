void HTMLCanvasElement::SetCanvas2DLayerBridgeInternal(
    std::unique_ptr<Canvas2DLayerBridge> external_canvas2d_bridge) {
  DCHECK(Is2d() && !canvas2d_bridge_);
  did_fail_to_create_resource_provider_ = true;

  if (!IsValidImageSize(Size()))
    return;

  if (external_canvas2d_bridge) {
    if (external_canvas2d_bridge->IsValid())
      canvas2d_bridge_ = std::move(external_canvas2d_bridge);
  } else {
    if (ShouldAccelerate(kNormalAccelerationCriteria))
      canvas2d_bridge_ = CreateAccelerated2dBuffer();
    if (!canvas2d_bridge_)
      canvas2d_bridge_ = CreateUnaccelerated2dBuffer();
  }

  if (canvas2d_bridge_)
    canvas2d_bridge_->SetCanvasResourceHost(this);
  else
    return;

  did_fail_to_create_resource_provider_ = false;
  UpdateMemoryUsage();

  if (!GetMSAASampleCountFor2dContext() && GetDocument().GetSettings() &&
      !GetDocument().GetSettings()->GetAntialiased2dCanvasEnabled()) {
    context_->SetShouldAntialias(false);
  }

  if (context_)
    SetNeedsCompositingUpdate();
}
