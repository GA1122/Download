void HTMLCanvasElement::WillDrawImageTo2DContext(CanvasImageSource* source) {
  if (canvas_heuristic_parameters::kEnableAccelerationToAvoidReadbacks &&
      SharedGpuContext::AllowSoftwareToAcceleratedCanvasUpgrade() &&
      source->IsAccelerated() && GetOrCreateCanvas2DLayerBridge() &&
      !canvas2d_bridge_->IsAccelerated() &&
      ShouldAccelerate(kIgnoreResourceLimitCriteria)) {
    std::unique_ptr<Canvas2DLayerBridge> surface = CreateAccelerated2dBuffer();
    if (surface) {
      ReplaceExisting2dLayerBridge(std::move(surface));
      SetNeedsCompositingUpdate();
    }
  }
}
